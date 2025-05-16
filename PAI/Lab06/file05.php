<?php
$link = mysqli_connect("localhost", "scott", "tiger", "instytut");
if (!$link) {
    printf("Connect failed: %s\n", mysql_connect_error());
    exit();
}
printf("<h1>Pracownicy</h1>");
$psql = "SELECT nazwisko, etat FROM pracownicy WHERE id_prac = ?";
$pstmt = mysqli_stmt_init($link);
mysqli_stmt_prepare($pstmt, $psql);
for ($i = 100; $i < 230; $i += 10) {
    mysqli_stmt_bind_param($pstmt, "i", $i);
    mysqli_stmt_execute($pstmt);
    mysqli_stmt_bind_result($pstmt, $nazwisko, $etat);
    mysqli_stmt_fetch($pstmt);
    printf("$nazwisko pracuje jako $etat <br/>");
}
mysqli_stmt_close($pstmt);
mysqli_close($link);
?>