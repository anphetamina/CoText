-- phpMyAdmin SQL Dump
-- version 5.0.3
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Creato il: Ott 26, 2020 alle 08:57
-- Versione del server: 10.4.14-MariaDB
-- Versione PHP: 7.4.11

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `cotext`
--

-- --------------------------------------------------------

--
-- Struttura della tabella `permission`
--

CREATE TABLE `permission` (
  `ID` int(11) NOT NULL,
  `UserID` int(11) DEFAULT NULL,
  `DocumentID` int(11) DEFAULT NULL,
  `DocumentName` varchar(255) NOT NULL,
  `DocumentPath` varchar(255) DEFAULT NULL,
  `URI` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `permission`
--

INSERT INTO `permission` (`ID`, `UserID`, `DocumentID`, `DocumentName`, `DocumentPath`, `URI`) VALUES
(2, 2, 12, 'AppuntiSofteng', NULL, NULL),
(3, 4, 12, 'AppuntiSofteng', NULL, NULL),
(4, 7, 12, 'AppuntiSofteng', NULL, NULL),
(5, 13, 12, 'AppuntiSofteng', NULL, NULL),
(6, 14, 12, 'AppuntiSofteng', NULL, NULL),
(8, 7, 33, 'Document1', NULL, NULL);

-- --------------------------------------------------------

--
-- Struttura della tabella `user`
--

CREATE TABLE `user` (
  `ID` int(11) NOT NULL,
  `Username` varchar(80) NOT NULL,
  `Password` varchar(256) NOT NULL,
  `Name` varchar(255) NOT NULL,
  `Surname` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dump dei dati per la tabella `user`
--

INSERT INTO `user` (`ID`, `Username`, `Password`, `Name`, `Surname`) VALUES
(2, 'test@test.test', 'test', 'Mario', 'Rossi'),
(3, 'test2@test2.test2', 'test2', 'Mario2', 'Rossi2'),
(4, 'test3@test3.test3', 'test3', 'Mario3', 'Rossi3'),
(5, 'ema.muna95@gmail.com', 'password', 'Emanuele', 'Munafo'),
(6, 'marco@marco.com', '12345', 'Marco', 'Testa'),
(7, 'tony.santoro@polito.it', '1234', 'Tony', 'Santoro'),
(8, 'ago@ago.it', 'ago', 'ago', 'ago');

--
-- Indici per le tabelle scaricate
--

--
-- Indici per le tabelle `permission`
--
ALTER TABLE `permission`
  ADD PRIMARY KEY (`ID`);

--
-- Indici per le tabelle `user`
--
ALTER TABLE `user`
  ADD PRIMARY KEY (`ID`),
  ADD UNIQUE KEY `Email` (`Username`);

--
-- AUTO_INCREMENT per le tabelle scaricate
--

--
-- AUTO_INCREMENT per la tabella `permission`
--
ALTER TABLE `permission`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;

--
-- AUTO_INCREMENT per la tabella `user`
--
ALTER TABLE `user`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
