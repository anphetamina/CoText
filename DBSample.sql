-- phpMyAdmin SQL Dump
-- version 4.9.3
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Creato il: Ott 01, 2020 alle 20:40
-- Versione del server: 5.7.26
-- Versione PHP: 7.2.22

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

--
-- Database: `CoText`
--

-- --------------------------------------------------------

--
-- Struttura della tabella `Permission`
--

CREATE TABLE `Permission` (
  `ID` int(11) NOT NULL,
  `UserID` int(11) DEFAULT NULL,
  `DocumentID` int(11) DEFAULT NULL,
  `DocumentPath` varchar(260) NOT NULL,
  `DocumentName` varchar(255) NOT NULL,
  `URI` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `Permission`
--

INSERT INTO `Permission` (`ID`, `UserID`, `DocumentID`, `DocumentPath`, `DocumentName`, `URI`) VALUES
(1, 1, 1, '', 'TestDocument1', ''),
(12, 1, 12, '', 'AAA', NULL),
(13, 2, 12, '', 'AAA', NULL),
(28, 3, 12, '', 'AAA', NULL),
(29, 4, 12, '', 'AAA', NULL);

-- --------------------------------------------------------

--
-- Struttura della tabella `User`
--

CREATE TABLE `User` (
  `ID` int(11) NOT NULL,
  `Username` varchar(80) NOT NULL,
  `Email` varchar(80) NOT NULL,
  `Password` varchar(256) NOT NULL,
  `Name` varchar(255) NOT NULL,
  `Surname` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `User`
--

INSERT INTO `User` (`ID`, `Username`, `Email`, `Password`, `Name`, `Surname`) VALUES
(1, 'test', 'test@test.test', 'test', 'Mario', 'Rossi'),
(2, 'tester', 'test@domain.tld', 'test2HashedPassword', 'Giovanni', 'Muciaccia'),
(3, 'marco', 'marco@marco.it', 'marco', 'marco', 'marco'),
(4, 'test2', 'test2@test2.test2', 'test2', 'Mario2', 'Rossi2'),
(5, 'test3', 'test3@test3.test3', 'test3', 'Mario3', 'Rossi3');

--
-- Indici per le tabelle scaricate
--

--
-- Indici per le tabelle `Permission`
--
ALTER TABLE `Permission`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `User`
--
ALTER TABLE `User`
  ADD PRIMARY KEY (`ID`);

--
-- AUTO_INCREMENT per le tabelle scaricate
--

--
-- AUTO_INCREMENT per la tabella `Permission`
--
ALTER TABLE `Permission`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=30;

--
-- AUTO_INCREMENT per la tabella `User`
--
ALTER TABLE `User`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
