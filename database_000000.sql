BEGIN TRANSACTION;
/*STATEMENT SEPARATOR*/
PRAGMA journal_mode = WAL;
/*STATEMENT SEPARATOR*/
PRAGMA synchronous = NORMAL;
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
    "tax-deduction"             REAL,
    "pension-insurance"         REAL,
    "medical-insurance"         REAL,
    "unemployment-insurance"    REAL,
    "housing-fund"              REAL,
    "housing-loan"              REAL,
    "house-renting"             REAL,
    "elderly-care-support"      REAL,
    "serious-illness-support"   REAL,
    "adult-education"           REAL,
    "children-education"        REAL,
    "infant-care-support"       REAL,
    "personal-pension"          REAL
);
/*STATEMENT SEPARATOR*/
CREATE TABLE IF NOT EXISTS "version" (
    "version-id"    INTEGER);
/*STATEMENT SEPARATOR*/
INSERT INTO "version" ("version-id") VALUES (0);
/*STATEMENT SEPARATOR*/
COMMIT;
