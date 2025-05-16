<?php
session_start();

// Turn on error handling
mysqli_report(MYSQLI_REPORT_ERROR | MYSQLI_REPORT_STRICT);

$link = mysqli_connect("localhost", "scott", "tiger", "instytut");
if (!$link) {
    printf("Connect failed: %s\n", mysqli_connect_error());
    exit();
}

if (isset($_POST['id_prac']) && is_numeric($_POST['id_prac']) && is_string($_POST['nazwisko'])) {
    $sql = "INSERT INTO pracownicy (id_prac, nazwisko) VALUES (?, ?)";
    $stmt = $link->prepare($sql);
    $stmt->bind_param("is", $_POST['id_prac'], $_POST['nazwisko']);

    try {
        $stmt->execute();
        $_SESSION['message'] = "Pracownik został dodany pomyślnie.";
        header("Location: form06_get.php");
        exit();
    } catch (mysqli_sql_exception $e) {
        if ($e->getCode() === 1062) {
            $_SESSION['error'] = "Pracownik o podanym ID już istnieje.";
        } else {
            $_SESSION['error'] = "Błąd podczas dodawania pracownika.";
        }
        header("Location: form06_post.php");
        exit();
    }
} else {
    $_SESSION['error'] = "Nieprawidłowe dane wejściowe.";
    header("Location: form06_post.php");
    exit();
}
?>