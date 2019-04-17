#!/bin/bash

# Convert the CSV format output by the DXR indexer plugin into a SQLlite database.
# The CSV contains key-value data, with keys in the even and values in the odd columns,
# the first column is the type of the reference.
# This script generates and runs some SQL statements to transform the data into one table of names columns.

# call as csv_to_sqlite.sh <all.csv>, where all.csv is the concatenation of all the csv files produced.

DB="sqlite3 ${1}.sqlite"

$DB 2> /dev/null <<EOF
CREATE TABLE dxrdata(thing, k1, v1, k2, v2, k3, v3, k4, v4, k5, v5, k6, v6, k7, v7, k8, v8);
.mode csv
.import $1 dxrdata
EOF

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

$DB <<EOF
CREATE TABLE dxrcols(thing,
$columns
);
EOF


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

$DB <<EOF
$inserts
DROP TABLE dxrdata;
EOF
