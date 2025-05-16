CREATE TABLE books (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(50),
    author VARCHAR(20),
    genre VARCHAR(20)
) DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

INSERT INTO books (title, author, genre) VALUES
('Wiedźmin', 'Andrzej Sapkowski', 'Fantasy'),
('Lśnienie', 'Stephen King', 'Horror'),
('Zabójstwo Rogera Ackroyda', 'Agatha Christie', 'Kryminał');

CREATE TABLE users (
    id INT(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(255) NOT NULL UNIQUE,
    password CHAR(40) NOT NULL,
    group_id INT(11) NOT NULL,
    created DATETIME,
    modified DATETIME
);

CREATE TABLE groups (
    id INT(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    created DATETIME,
    modified DATETIME
);

DROP TABLE IF EXISTS `employees`;
CREATE TABLE `employees` (
    `id` int AUTO_INCREMENT NOT NULL,
    `nazwisko` varchar(15) default NULL,
    `imie` varchar(15) default NULL,
    `etat` varchar(10) default NULL,
    `id_szefa` decimal(4,0) default NULL,
    `zatrudniony` date default NULL,
    `placa_pod` decimal(6,2) default NULL,
    `placa_dod` decimal(6,2) default NULL,
    `id_zesp` decimal(2,0) default NULL,
    PRIMARY KEY (`id`)
) ENGINE=MyISAM;

INSERT INTO `employees` (`id`, `nazwisko`, `imie`, `etat`, `id_szefa`,
`zatrudniony`, `placa_pod`, `placa_dod`, `id_zesp`)
VALUES
(100, 'Marecki', 'Jan', 'DYREKTOR', NULL, '1968-01-01', 4730.00, 980.50, 10),
(110, 'Janicki', 'Karol', 'PROFESOR', 100, '1973-05-01', 3350.00, 610.00, 40),
(120, 'Nowicki', 'Pawel', 'PROFESOR', 100, '1977-09-01', 3070.00, NULL, 30),
(130, 'Nowak', 'Piotr', 'PROFESOR', 100, '1968-07-01', 3960.00, NULL, 20),
(140, 'Kowalski', 'Krzysztof', 'PROFESOR', 130, '1975-09-15', 3230.00, 805.00, 20),
(150, 'Grzybowska', 'Maria', 'ADIUNKT', 130, '1977-09-01', 2845.50, NULL, 20),
(160, 'Krakowska', 'Joanna', 'SEKRETARKA', 130, '1985-03-01', 1590.00, NULL, 20),
(170, 'Opolski', 'Roman', 'ASYSTENT', 130, '1992-10-01', 1839.70, 480.50, 20),
(190, 'Kotarski', 'Konrad', 'ASYSTENT', 140, '1993-09-01', 1971.00, NULL, 20),
(180, 'Makowski', 'Marek', 'ADIUNKT', 100, '1985-02-20', 2610.20, NULL, 10),
(200, 'Przywarek', 'Leon', 'DOKTORANT', 140, '1994-07-15', 900.00, NULL, 30),
(210, 'Kotlarczyk', 'Stefan', 'DOKTORANT', 130, '1993-10-15', 900.00, 570.60, 30),
(220, 'Siekierski', 'Mateusz', 'ASYSTENT', 110, '1993-10-01', 1889.00, NULL, 20),
(230, 'Dolny', 'Tomasz', 'ASYSTENT', 120, '1992-09-01', 1850.00, 390.00, NULL);