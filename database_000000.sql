PRAGMA journal_mode = WAL;
PRAGMA synchronous = NORMAL;
BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "income" (
    "year"      INTEGER,
    "month"     INTEGER,
    "income"    REAL DEFAULT 0.0,
    "bonus"     REAL DEFAULT 0.0,
    "accidental-income" REAL DEFAULT 0.0
);
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
CREATE TABLE IF NOT EXISTS "version" (
    "version-id"    INTEGER);
INSERT INTO "version" ("version-id") VALUES (0);
COMMIT;
