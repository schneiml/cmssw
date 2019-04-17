#!/bin/bash

# Convert the CSV format output by the DXR indexer plugin into a SQLlite database.

report () {
  echo "$@" 1>&2
  "$@"
}

find . -maxdepth 1 -name '*.csv' | xargs cat > allcsv

rm -f dxr.sqlite
DB="sqlite3 dxr.sqlite"

# First, import the CSV file. 
$DB 2> /dev/null <<EOF
CREATE TABLE dxrdata(thing, k1, v1, k2, v2, k3, v3, k4, v4, k5, v5, k6, v6, k7, v7, k8, v8);
.mode csv
.import allcsv dxrdata
EOF

# The CSV contains key-value data, with keys in the even and values in the odd columns,
# the first column is the type of the reference.
# This script generates and runs some SQL statements to transform the data into one table of names columns.
columns=$( ($DB 2> /dev/null ; echo dummy) <<EOF
SELECT DISTINCT k1 || "," FROM (
        SELECT DISTINCT k1 FROM dxrdata 
  UNION SELECT DISTINCT k2 FROM dxrdata 
  UNION SELECT DISTINCT k3 FROM dxrdata 
  UNION SELECT DISTINCT k4 FROM dxrdata 
  UNION SELECT DISTINCT k4 FROM dxrdata 
  UNION SELECT DISTINCT k5 FROM dxrdata 
  UNION SELECT DISTINCT k6 FROM dxrdata 
  UNION SELECT DISTINCT k7 FROM dxrdata 
  UNION SELECT DISTINCT k8 FROM dxrdata
) WHERE k1 is not Null;
EOF
)

# Then, make a table with these columns.
$DB <<EOF
CREATE TABLE dxrcols(thing,
$columns
);
EOF

# Finally, make statements that transform the data
inserts=$($DB <<EOF
SELECT DISTINCT "INSERT INTO dxrcols(thing" ||
  CASE WHEN k1 IS NOT NULL THEN "," || k1 ELSE "" END ||
  CASE WHEN k2 IS NOT NULL THEN "," || k2 ELSE "" END ||
  CASE WHEN k3 IS NOT NULL THEN "," || k3 ELSE "" END ||
  CASE WHEN k4 IS NOT NULL THEN "," || k4 ELSE "" END ||
  CASE WHEN k5 IS NOT NULL THEN "," || k5 ELSE "" END ||
  CASE WHEN k6 IS NOT NULL THEN "," || k6 ELSE "" END ||
  CASE WHEN k7 IS NOT NULL THEN "," || k7 ELSE "" END ||
  CASE WHEN k8 IS NOT NULL THEN "," || k8 ELSE "" END ||
") SELECT thing" ||
  CASE WHEN k1 IS NOT NULL THEN ", v1" ELSE "" END ||
  CASE WHEN k2 IS NOT NULL THEN ", v2" ELSE "" END ||
  CASE WHEN k3 IS NOT NULL THEN ", v3" ELSE "" END ||
  CASE WHEN k4 IS NOT NULL THEN ", v4" ELSE "" END ||
  CASE WHEN k5 IS NOT NULL THEN ", v5" ELSE "" END ||
  CASE WHEN k6 IS NOT NULL THEN ", v6" ELSE "" END ||
  CASE WHEN k7 IS NOT NULL THEN ", v7" ELSE "" END ||
  CASE WHEN k8 IS NOT NULL THEN ", v8" ELSE "" END ||
  " FROM dxrdata WHERE thing = '" || thing || "' " ||
  CASE WHEN k1 IS NOT NULL THEN "and k1 = '" || k1 || "' " ELSE "and k1 is null " END ||
  CASE WHEN k2 IS NOT NULL THEN "and k2 = '" || k2 || "' " ELSE "and k2 is null " END ||
  CASE WHEN k3 IS NOT NULL THEN "and k3 = '" || k3 || "' " ELSE "and k3 is null " END ||
  CASE WHEN k4 IS NOT NULL THEN "and k4 = '" || k4 || "' " ELSE "and k4 is null " END ||
  CASE WHEN k5 IS NOT NULL THEN "and k5 = '" || k5 || "' " ELSE "and k5 is null " END ||
  CASE WHEN k6 IS NOT NULL THEN "and k6 = '" || k6 || "' " ELSE "and k6 is null " END ||
  CASE WHEN k7 IS NOT NULL THEN "and k7 = '" || k7 || "' " ELSE "and k7 is null " END ||
  CASE WHEN k8 IS NOT NULL THEN "and k8 = '" || k8 || "' " ELSE "and k8 is null " END ||
";" FROM (SELECT DISTINCT thing, k1, k2, k3, k4, k5, k6, k7, k8 FROM dxrdata);
EOF
)

# and execute them.
$DB <<EOF
$inserts
CREATE INDEX things on dxrcols(thing);
DROP TABLE dxrdata;
EOF

# Now, we can split the big table into multiple small ones.
# Make one per type of row, check with a qurery for each column if there are 
# any non-null values, and cut column numbers from the loc values.
# Also add an index for each loc column for future use.
things=$($DB <<EOF
SELECT DISTINCT thing FROM dxrcols;
EOF
)
for t in $things; do 
  cols=""
  selects=""
  index=""
  for c in $columns; do
    used=$( $DB <<EOF
    SELECT max(${c%,}) FROM dxrcols WHERE thing="$t";
EOF
    )
    if [ ! -z "$used" ]; then
      echo $c used in $t >&2
      cols="$cols $c"
      if [[ $c == *loc, ]]; then
        echo "  " $c is loc >&2
        selects="$selects rtrim(rtrim(${c%,}, '0123456789'), ':'),"
        index="$index CREATE INDEX ${t}_${c%,} ON $t(${c%,});"
      else
        selects="$selects $c"
      fi

    fi
  done
  echo "CREATE TABLE $t($cols dummy);"
  echo "$index"
  echo "INSERT INTO $t SELECT $selects dummy FROM dxrcols WHERE thing='$t';" 
done > transform
$DB < transform

# Add source code into the DB.
$DB <<EOF
CREATE TABLE code(loc, line);
CREATE INDEX codeloc ON code(loc);
EOF

for f in $($DB <<EOF
  SELECT DISTINCT rtrim(loc, '0123456789:') FROM ref;
EOF
); do
  echo "Importing $f ..."
  $DB <<EOF
CREATE TEMP TABLE codefile(line);
-- some very rare character, we don't want more than one column
.separator "\v"
.import $f codefile
INSERT INTO code SELECT "$f:" || rowid as loc, line FROM codefile;
EOF
done
