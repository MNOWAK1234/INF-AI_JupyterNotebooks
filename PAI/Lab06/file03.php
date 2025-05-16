<?php
$link = mysqli_connect("localhost", "scott", "tiger", "instytut");
if (!$link) {
    printf("Connect failed: %s\n", mysqli_connect_error());
    exit();
}
$result = mysqli_query($link, "SELECT * FROM etaty ORDER BY placa_od");
if (!$result) {
    printf("Query failed: %s\n", mysqli_error($link));
}
printf("<h1>ETATY</h1>");
printf("<table border=\"1\">");
printf("<tr><th>NAZWA</th><th>PLACA_OD</th><th>PLACA_DO</th></tr>");
while ($row = mysqli_fetch_assoc($result))
    printf(
        "<tr><td>%s</td><td>%6.2f</td><td>%6.2f</td></tr>",
        $row["NAZWA"],
        $row["PLACA_OD"],
        $row["PLACA_DO"]
    );
printf("</table>");
printf("<i>query returned %d rows </i>", mysqli_num_rows($result));
mysqli_free_result($result);
mysqli_close($link);
?>