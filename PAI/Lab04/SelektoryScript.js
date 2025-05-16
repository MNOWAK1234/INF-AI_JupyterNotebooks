$(document).ready(function () {
  $("#a").css("color", "red").append(" doklejone");
  $("div").css("color", "green");
  $(".c").css("font-size", "x-large");
  $("tr + tr").css("color", "aqua");
  $("table tr:first").css("color", "yellow");
  $("tr:last").css("color", "yellow");
  $("p:even").css("color", "orange");
  $("[href]").css("border", "1px solid black").css("padding", "3px");
  $("a[href*=kalaka]").css("border", "1px solid pink");
  $("a[href$='.pdf']").css("border", "1px solid red");
  alert('Wartość atrybutu color elementu o id="a" to ' + $("#a").css("color"));
});
