<?php
session_start();

$link = mysqli_connect("localhost", "scott", "tiger", "instytut");
if (!$link) {
    printf("Connect failed: %s\n", mysqli_connect_error());
    exit();
}

?>

<html>

<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
</head>

<body>
    <?php
    if (isset($_SESSION['message'])) {
        echo $_SESSION['message'];
        unset($_SESSION['message']);
    }
    if (isset($_SESSION['error'])) {
        echo $_SESSION['error'];
        unset($_SESSION['error']);
    }
    ?>

    <table>
        <tr>
            <th>id_prac</th>
            <th>nazwisko</th>
        </tr>
        <?php
        $sql = "SELECT * FROM pracownicy";
        $result = $link->query($sql);
        foreach ($result as $v) {
            echo "<tr>";
            echo "<td>" . $v["ID_PRAC"] . "</td>";
            echo "<td>" . $v["NAZWISKO"] . "</td>";
            echo "</tr>";
        }
        $result->free();
        ?>
    </table>
    <a href="form06_post.php">Dodaj nowego pracownika</a>
</body>

</html>