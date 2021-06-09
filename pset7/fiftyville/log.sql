-- Keep a log of any SQL queries you execute as you solve the mystery.

-- See what is the description of crime in question. We know the date and place (July 28, 2020 Chamberlin Street)
SELECT description FROM crime_scene_reports
WHERE year = 2020 AND month = 7 AND day = 28 AND street = "Chamberlin Street";
--Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. Interviews were conducted
--today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.


-- See what whitnesses say in their interviews
SELECT name, transcript FROM interviews
WHERE year = 2020 AND month = 7 AND day = 28;
--Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot
--and drive away. If you have security footage from the courthouse parking lot, you might want to look for cars
--that left the parking lot in that time frame.
-- Eugene | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived
--at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
-- Raymond | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute.
--In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
--The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- See courthouse_security_logs on that day
-- COUNT(DISTINCT license_plate) 37 unique cars
SELECT hour, minute, activity, license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28;

-- See who was driving from parking lot at courthouse
SELECT DISTINCT name from people
JOIN courthouse_security_logs ON people.license_plate = courthouse_security_logs.license_plate
WHERE courthouse_security_logs.license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28)
ORDER BY name;
-- 37 names


-- See what accounts was withdrawed in atm_transactions on that day
SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28 AND atm_location = "Fifer Street" AND transaction_type = "withdraw";
-- 8 accounts


-- Find the names of account owners who withdraw money
SELECT DISTINCT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_transactions.account_number IN (SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28 AND atm_location = "Fifer Street" AND transaction_type = "withdraw")
ORDER BY name;
-- Bobby
-- Danielle
-- Elizabeth
-- Ernest
-- Madison
-- Roy
-- Russell
-- Victoria

-- See caller number in phone_calls on that day which lasted less then 1 min
SELECT DISTINCT caller FROM phone_calls
WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60;


-- See caller name
SELECT DISTINCT name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE phone_calls.caller IN (SELECT DISTINCT caller FROM phone_calls
WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60)
ORDER BY name;
-- Bobby
-- Ernest
-- Evelyn
-- Kimberly
-- Madison
-- Roger
-- Russell
-- Victoria

-- Check if caller withdraw money from ATM
SELECT DISTINCT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_transactions.account_number IN (SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28 AND atm_location = "Fifer Street" AND transaction_type = "withdraw")
INTERSECT
SELECT DISTINCT name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE phone_calls.caller IN (SELECT DISTINCT caller FROM phone_calls
WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60)
ORDER BY name;
-- These are SUSPECTS
-- Bobby
-- Ernest
-- Madison
-- Russell
-- Victoria


--Check earliest fligth from Fiftyville tommorrow

-- SELECT * FROM flights
-- WHERE year = 2020 AND month = 7 AND day = 29 AND origin_airport_id IN (SELECT id FROM airports WHERE city = "Fiftyville")
-- ORDER BY hour ASC, minute ASC
-- LIMIT 1;

SELECT flights.id, airports.city, flights.hour, flights.minute FROM airports
JOIN flights ON airports.id = flights.destination_airport_id
WHERE destination_airport_id IN
(SELECT destination_airport_id FROM airports
JOIN flights ON airports.id = flights.origin_airport_id
WHERE year = 2020 AND month = 7 AND day = 29 AND city = "Fiftyville"
ORDER BY hour ASC, minute ASC
LIMIT 1)
ORDER BY hour ASC, minute ASC
LIMIT 1;


-- See who passengers of earliest flight from Fiftyville tommorrow are
SELECT name from people
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
WHERE flights.id IN
(SELECT flights.id FROM airports
JOIN flights ON airports.id = flights.destination_airport_id
WHERE destination_airport_id IN
(SELECT destination_airport_id FROM airports
JOIN flights ON airports.id = flights.origin_airport_id
WHERE year = 2020 AND month = 7 AND day = 29 AND city = "Fiftyville"
ORDER BY hour ASC, minute ASC
LIMIT 1)
ORDER BY hour ASC, minute ASC
LIMIT 1)
ORDER BY name;
-- Bobby
-- Danielle
-- Doris
-- Edward
-- Ernest
-- Evelyn
-- Madison
-- Roger

--See if any of previous 5 suspects have tickets for tommorow AND whether or not they were driving from parking lot
SELECT name from people
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
WHERE flights.id IN
(SELECT flights.id FROM airports
JOIN flights ON airports.id = flights.destination_airport_id
WHERE destination_airport_id IN
(SELECT destination_airport_id FROM airports
JOIN flights ON airports.id = flights.origin_airport_id
WHERE year = 2020 AND month = 7 AND day = 29 AND city = "Fiftyville"
ORDER BY hour ASC, minute ASC
LIMIT 1)
ORDER BY hour ASC, minute ASC
LIMIT 1)
INTERSECT
SELECT DISTINCT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_transactions.account_number IN (SELECT account_number FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28 AND atm_location = "Fifer Street" AND transaction_type = "withdraw")
INTERSECT
SELECT DISTINCT name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE phone_calls.caller IN (SELECT DISTINCT caller FROM phone_calls
WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60)
INTERSECT
SELECT DISTINCT name from people
JOIN courthouse_security_logs ON people.license_plate = courthouse_security_logs.license_plate
WHERE courthouse_security_logs.license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28)
ORDER BY name;
--ONLY 2 SUSPECTS left
-- Ernest
-- Madison


-- Check what time suspects were driving at courthouse_security_logs (maybe it was not morning?)
SELECT DISTINCT people.name, courthouse_security_logs.hour, courthouse_security_logs.minute, courthouse_security_logs.activity from people
JOIN courthouse_security_logs ON people.license_plate = courthouse_security_logs.license_plate
WHERE name = "Ernest" OR name = "Madison" AND courthouse_security_logs.license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28)
ORDER BY name;
--name | hour | minute | activity
-- Ernest | 8 | 23 | entrance
-- Ernest | 10 | 18 | exit
-- Madison | 8 | 34 | entrance
-- Madison | 10 | 35 | exit


-- Check the name who was suspects calling to (SELF JOIN people name on 2 foreign keys)
SELECT DISTINCT a.name as caller, b.name as receiver, phone_calls.day, phone_calls.duration FROM people A
JOIN people B ON b.phone_number = phone_calls.receiver
JOIN phone_calls ON a.phone_number = phone_calls.caller
WHERE a.name = "Ernest" AND year = 2020 AND month = 7 AND day = 28 AND duration < 60
OR a.name = "Madison" AND year = 2020 AND month = 7 AND day = 28 AND duration < 60
AND phone_calls.caller IN (SELECT DISTINCT caller FROM phone_calls
WHERE year = 2020 AND month = 7 AND day = 28 AND duration < 60);
-- These are ACCOMPLICES
-- caller | receiver | day | duration
-- Madison | James | 28 | 43
-- Ernest | Berthold | 28 | 45

--check if ACCOMPLICES have bank account to buy the flight ticket
SELECT people.name, bank_accounts.account_number FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE name = "James" OR name = "Berthold";
-- name | account_number
-- Berthold | 94751264

-- SO THE ACCOMPLICE TURNS OUT TO BE BERTHOLD MEANING THE THIEF IS ERNEST :)