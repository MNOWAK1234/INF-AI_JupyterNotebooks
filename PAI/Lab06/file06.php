<?php
$link = mysqli_connect("localhost", "scott", "tiger", "instytut");
if (!$link) {
    printf("Connect failed: %s\n", mysqli_connect_error());
    exit();
}
print <<<KONIEC
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
</head>
<body>
<form action="file06.php" method="POST">
id_prac <input type="text" name="id_prac">
nazwisko <input type="text" name="nazwisko">
<input type="submit" value="Wstaw">
<input type="reset" value="Wyczysc">
</form>
KONIEC;
if (
    isset($_POST['id_prac']) &&
    is_numeric($_POST['id_prac']) &&
    is_string($_POST['nazwisko'])
) {
    $sql = "INSERT INTO pracownicy(id_prac,nazwisko) VALUES(?,?)";
    $stmt = $link->prepare($sql);
    $stmt->bind_param("is", $_POST['id_prac'], $_POST['nazwisko']);
    $result = $stmt->execute();
    if (!$result) {
        printf("Query failed: %s\n", mysqli_error($link));
    }
    $stmt->close();
}
$sql = "SELECT * FROM pracownicy";
$result = $link->query($sql);
foreach ($result as $v) {
    echo $v["ID_PRAC"] . " " . $v["NAZWISKO"] . "<br/>";
}
$result->free();
$link->close();
?>