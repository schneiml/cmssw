(sqlite3 $1 | column -t -s'|') <<SQL
.header on
.mode csv
.separator |
.nullvalue null
SELECT groups.*, qualname FROM (
  SELECT count(*) as '#refs', count(distinct locpkg) as '#users', max(locpkg) as 'user', kind, name, def FROM (
    SELECT thing, defloc, loc, kind, name, 
      CASE WHEN instr(defloc, "0/src") THEN substr(defloc, instr(defloc, "0/src/")+6) ELSE defloc END as def, 
      substr(replace(replace(loc, "/plugins/", "/src/"), "/interface/", "/src/"), 0, instr(replace(replace(loc, "/plugins/", "/src/"), "/interface/", "/src/"), "/src")) as locpkg 
    FROM dxrcols
  ) WHERE  thing="ref" AND instr(defloc, "DQMServices") AND not instr(loc, "DQMService") GROUP BY def
) as groups, ( 
  SELECT DISTINCT loc, qualname FROM dxrcols
) as names WHERE instr(names.loc, groups.def)
ORDER BY groups.'#users' DESC;
SQL
