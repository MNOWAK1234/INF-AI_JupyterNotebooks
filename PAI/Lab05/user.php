<?php
session_start();

// Check if the user is logged in, if not, redirect to the login page
if (!isset($_SESSION['zalogowany']) || $_SESSION['zalogowany'] !== 1) {
    header("Location: index.php");
    exit();
}

// Handle logout form submission
if (isset($_POST['wyloguj'])) {
    // Destroy the session and redirect to the login page
    session_destroy();
    header("Location: index.php");
    exit();
}

// Handle file submission
if (isset($_FILES['myfile'])) {
    $currentDir = getcwd();
    $uploadDirectory = "/images/";
    $fileName = $_FILES['myfile']['name'];
    $fileSize = $_FILES['myfile']['size'];
    $fileTmpName = $_FILES['myfile']['tmp_name'];
    $fileType = $_FILES['myfile']['type'];

    // Check if the uploaded file is a jpg or png image
    if ($fileName != "" && ($fileType == 'image/png' || $fileType == 'image/jpeg')) {
        $uploadPath = $currentDir . $uploadDirectory . $fileName;

        // Upload the file to the server
        if (move_uploaded_file($fileTmpName, $uploadPath)) {
            echo "Zdjęcie zostało załadowane na serwer FTP<br>";
            echo "<img src='$uploadDirectory$fileName' alt='Zdjęcie użytkownika'>";
        } else {
            echo "Wystąpił problem podczas przesyłania pliku.";
        }
    } else {
        echo "Niepoprawny format pliku. Akceptowane są tylko pliki JPG i PNG.";
    }
}
?>

<!DOCTYPE html>
<html>

<head>
    <title>Strona użytkownika</title>
    <meta charset="UTF-8">
</head>

<body>

    <h1>Witaj, <?php echo $_SESSION['zalogowanyImie']; ?>!</h1>

    <p>To jest strona użytkownika. Tutaj możesz zobaczyć swoje dane.</p>

    <!-- Form for uploading a file -->
    <form action="user.php" method="POST" enctype="multipart/form-data">
        <input type="file" name="myfile">
        <input type="submit" value="Prześlij plik" name="upload">
    </form>

    <form action="user.php" method="POST">
        <input type="submit" name="wyloguj" value="Wyloguj">
    </form>

    <a href="index.php">Wróć do strony logowania</a>

</body>

</html>