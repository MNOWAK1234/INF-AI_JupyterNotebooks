<?php
$link = mysqli_connect("localhost", "scott", "tiger", "instytut");
if (!$link) {
    printf("Connect failed: %s\n", mysqli_connect_error());
    exit();
}
$result = mysqli_query($link, "SELECT * FROM zespoly");
if (!$result) {
    printf("Query failed: %s\n", mysqli_error($link));
}
printf("<h1>ZESPOLY</h1>");
printf("<table border=\"1\">");
$fields = mysqli_fetch_fields($result);
printf(
    "<tr><th>%s</th><th>%s</th><th>%s</th></tr>",
    $fields[0]->name,
    $fields[1]->name,
    $fields[2]->name
);
while ($row = mysqli_fetch_array($result))
    printf("<tr><td>%s</td><td>%s</td><td>%s</td></tr>", $row[0], $row[1], $row[2]);
printf("</table>");
printf("<i>query returned %d rows </i>", mysqli_num_rows($result));
mysqli_free_result($result);
mysqli_close($link);
?>