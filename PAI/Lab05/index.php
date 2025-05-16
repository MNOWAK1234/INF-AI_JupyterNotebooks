<!DOCTYPE html>
<html>

<head>
    <title>Formularz logowania - Odbiór danych</title>
    <meta charset="UTF-8">
</head>

<body>

    <h1>Nasz system</h1>

    <fieldset>
        <legend>Logowanie i nawigacja użytkownika</legend>

        <?php
        session_start();
        require ("funkcje.php");

        // Check if the user is logged in
        if (isset($_SESSION['zalogowany']) && $_SESSION['zalogowany'] === 1) {
            echo "<p>Jesteś zalogowany jako: {$_SESSION['zalogowanyImie']}</p>";
        }

        // Display message about incorrect login data
        if (isset($_SESSION['bladLogowania'])) {
            echo $_SESSION['bladLogowania'];
            unset($_SESSION['bladLogowania']);
        }

        ?>

        <form action="logowanie.php" method="POST">
            <label for="login">Login:</label><br>
            <input type="text" id="login" name="login"><br>

            <label for="haslo">Hasło:</label><br>
            <input type="password" id="haslo" name="haslo"><br>

            <input type="submit" value="Zaloguj" name="zaloguj">
        </form>

        <br>
        <a href="user.php">Przejdź do strony użytkownika</a>

    </fieldset>

    <fieldset>
        <legend>Formularz Tworzenia ciasteczka oraz obecny stan ciasteczka</legend>
        <form action="cookie.php" method="POST">
            <label for="wartosc">Wartość ciasteczka:</label><br>
            <input type="text" id="wartosc" name="wartosc"><br>

            <label for="czas">Czas życia ciasteczka (w sekundach):</label><br>
            <input type="number" id="czas" name="czas"><br>

            <input type="submit" value="Utwórz Cookie" name="utworzCookie"><br>
            <br>
            <!-- Div for displaying countdown time -->
            <div id="czas_odliczania"> Pozostały czas życia ciasteczka: 0 godz. 0 min. 0 sek.</div>
        </form>

        <?php
        if (isset($_COOKIE['moje_cookie'])) {
            echo "<p>Wartość cookie: {$_COOKIE['moje_cookie']}</p>";
            $czas_wygasniecia = $_COOKIE['moje_cookie'] + time();
            $czas_aktualny = time();
            $pozostaly_czas = $czas_wygasniecia - $czas_aktualny;

            if ($pozostaly_czas > 0) {
                // Calculate remaining time in hours, minutes, and seconds
                $godziny = floor($pozostaly_czas / 3600);
                $pozostaly_czas %= 3600;
                $minuty = floor($pozostaly_czas / 60);
                $sekundy = $pozostaly_czas % 60;
            }
        } else {
            echo "<p>Cookie nie istnieje lub wygasło.</p>";
        }
        ?>
    </fieldset>
    <script>
        // Countdown time in the browser
        <?php if (isset($_COOKIE['moje_cookie'])): ?>
            var czas = <?php echo $_COOKIE['moje_cookie']; ?>;
            var timer = setInterval(function () {
                czas--;
                var godziny = Math.floor(czas / 3600);
                var minuty = Math.floor((czas % 3600) / 60);
                var sekundy = czas % 60;
                document.getElementById('czas_odliczania').innerHTML = 'Pozostały czas życia ciasteczka: ' + godziny + ' godz. ' + minuty + ' min. ' + sekundy + ' sek.';
                if (czas <= 0) {
                    clearInterval(timer);
                    document.getElementById('czas_odliczania').innerHTML = 'Ciasteczko wygasło.';
                }
            }, 1000);
        <?php endif; ?>
    </script>
</body>

</html>