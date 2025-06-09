-- Właściwe Parametry katalogów
SET input_dir3 = '${hivevar:input_dir3}'; -- Skatalogowanie wyników MapReduce
SET input_dir4 = '${hivevar:input_dir4}'; -- Zbiór danych 4
SET output_dir6 = '${hivevar:output_dir6}'; -- Skatalogowanie wyników końcowych

--Czyszczenie bazy danych
DROP TABLE IF EXISTS leagues_info;
DROP TABLE IF EXISTS mapreduce_results;
DROP TABLE IF EXISTS league_data_combined;
DROP TABLE IF EXISTS top_3_leagues_per_level;
DROP TABLE IF EXISTS json_formatted_results;

-- Tworzenie tabel tymczasowych dla plików wejściowych
CREATE EXTERNAL TABLE IF NOT EXISTS mapreduce_results (
  league_id STRING,
  avg_wage DOUBLE,
  avg_age DOUBLE,
  count_players STRING
)
ROW FORMAT DELIMITED
FIELDS TERMINATED BY ';'
STORED AS TEXTFILE
LOCATION '${hivevar:input_dir3}';

CREATE EXTERNAL TABLE IF NOT EXISTS leagues_info (
  league_id STRING,
  league_name STRING,
  league_level INT
)
ROW FORMAT DELIMITED
FIELDS TERMINATED BY ';'
STORED AS TEXTFILE
LOCATION '${hivevar:input_dir4}';

-- Tworzenie tabeli połączonej dla wyników MapReduce i danych lig
CREATE TABLE IF NOT EXISTS league_data_combined AS
SELECT
  m.league_id AS league_id,
  l.league_name AS league_name,
  l.league_level AS league_level,
  m.avg_wage AS avg_wage,
  m.avg_age AS avg_age,
  CAST(REGEXP_REPLACE(m.count_players, '^[\t\n\r]+|[\t\n\r]+$', '') AS INT) AS count_players
FROM
  mapreduce_results m
JOIN
  leagues_info l
ON
  m.league_id = l.league_id;

-- Wybranie top 3 lig z najwyższymi zarobkami na poziom ligi
CREATE TABLE IF NOT EXISTS top_3_leagues_per_level AS
SELECT *
FROM (
  SELECT
    league_id,
    league_name,
    league_level,
    avg_wage,
    avg_age,
    count_players,
    ROW_NUMBER() OVER (PARTITION BY league_level ORDER BY avg_wage DESC) AS rank
  FROM league_data_combined
  WHERE league_level IS NOT NULL  -- Filtrujemy rekordy z NULL w league_level
) ranked_leagues
WHERE rank <= 3;

-- Tworzymy tabelę połączoną dla wyników i wybieramy kolumny
CREATE TABLE IF NOT EXISTS json_formatted_results AS
SELECT
  CONCAT(
    '{"league_id":"', league_id,
    '","league_name":"', league_name,
    '","league_level":', league_level,
    ',"avg_wage":', avg_wage,
    ',"avg_age":', avg_age,
    ',"count_players":', count_players, '}'
  ) AS json_line
FROM top_3_leagues_per_level;

-- Zapisanie danych w formacie JSON Lines do katalogu wyjściowego
INSERT OVERWRITE DIRECTORY '${hivevar:output_dir6}'
ROW FORMAT DELIMITED
FIELDS TERMINATED BY '\n'
STORED AS TEXTFILE
SELECT json_line
FROM json_formatted_results;