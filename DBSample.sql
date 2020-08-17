-- phpMyAdmin SQL Dump
-- version 4.9.3
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Aug 17, 2020 at 02:28 PM
-- Server version: 5.7.26
-- PHP Version: 7.2.22

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

--
-- Database: `CoText`
--
CREATE DATABASE IF NOT EXISTS `CoText` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
USE `CoText`;

-- --------------------------------------------------------

--
-- Table structure for table `Permission`
--

CREATE TABLE `Permission` (
  `ID` int(11) NOT NULL,
  `UserID` int(11) NOT NULL,
  `DocumentID` int(11) DEFAULT NULL,
  `DocumentPath` varchar(260) NOT NULL,
  `DocumentName` varchar(255) NOT NULL,
  `URI` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Permission`
--

INSERT INTO `Permission` (`ID`, `UserID`, `DocumentID`, `DocumentPath`, `DocumentName`, `URI`) VALUES
(1, 1, 1, '', 'TestDocument1', ''),
(12, 1, 12, '', 'AAA', NULL);

-- --------------------------------------------------------

--
-- Table structure for table `User`
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
-- Dumping data for table `User`
--

INSERT INTO `User` (`ID`, `Username`, `Email`, `Password`, `Name`, `Surname`) VALUES
(1, 'test', 'test@test.test', 'test', 'Mario', 'Rossi'),
(2, 'tester', 'test@domain.tld', 'test2HashedPassword', 'Giovanni', 'Muciaccia');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `Permission`
--
ALTER TABLE `Permission`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `User`
--
ALTER TABLE `User`
  ADD PRIMARY KEY (`ID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `Permission`
--
ALTER TABLE `Permission`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=13;

--
-- AUTO_INCREMENT for table `User`
--
ALTER TABLE `User`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

