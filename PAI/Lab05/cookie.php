<?php
// Receive the submitted cookie value and cookie lifetime
if (isset($_POST['utworzCookie'])) {
    $wartosc = $_POST['wartosc'];
    $czas = $_POST['czas'];

    // Create a cookie with the specified value and lifetime
    setcookie("moje_cookie", $wartosc, time() + $czas, "/");

    // Pass the cookie value to the index.php file using a session
    $_SESSION['wartosc_ciasteczka'] = $wartosc;

    // Display success message
    echo "Ciasteczko zostało dodane pomyślnie.";
}
?>

<!DOCTYPE html>
<html>

<head>
    <title>Strona z ciasteczkami</title>
    <meta charset="UTF-8">
</head>

<body>

    <h1>Strona z ciasteczkami</h1>

    <a href="index.php">Wstecz</a>

</body>

</html>