BEGIN TRANSACTION;
/*STATEMENT SEPARATOR*/
CREATE TABLE IF NOT EXISTS "income" (
    "year"      INTEGER,
    "month"     INTEGER,
    "income"    REAL,
    "bonus"     REAL
);
/*STATEMENT SEPARATOR*/
CREATE TABLE IF NOT EXISTS "deduction" (
    "year"                      INTEGER,
    "month"                     INTEGER,
    "threshold"                 REAL,
    "pension"                   REAL,
    "medical-insurance"         REAL,
    "unemployment-insurance"    REAL,
    "housing-fund"              REAL,
    "housing-loan"              REAL,
    "house-renting"             REAL,
    "elderly-support"           REAL,
    "serious-illness-support"   REAL,
    "adult-education"           REAL,
    "children-education"        REAL,
    "personal-pension"          REAL
);
/*STATEMENT SEPARATOR*/
CREATE TABLE IF NOT EXISTS "version" (
    "version_id"    INTEGER);
/*STATEMENT SEPARATOR*/
INSERT INTO version (version_id) VALUES (0);
/*STATEMENT SEPARATOR*/
COMMIT;