function isWhiteSpaceOrEmpty(str) {
  return /^[\s\t\r\n]*$/.test(str);
}

/*function checkString(str, errorMessage) {
  if (isWhiteSpaceOrEmpty(str)) {
    alert(errorMessage);
    return false;
  } else {
    return true;
  }
}*/

function IsEmailInvalid(str) {
  let email = /^[a-zA-Z_0-9\.]+@[a-zA-Z_0-9\.]+\.[a-zA-Z][a-zA-Z]+$/;
  return !email.test(str);
}

function checkAndFocus(obj, msg, func) {
  let str = obj.value;
  let errorFieldName = "e_" + obj.name.substr(2, obj.name.length);
  if (func(str)) {
    document.getElementById(errorFieldName).innerHTML = msg;
    obj.focus(); // Ustawia fokus na polu formularza
    return false;
  } else {
    document.getElementById(errorFieldName).innerHTML = "";
    return true;
  }
}

function validate(form) {
  var imie = form.elements["f_imie"];
  var nazwisko = form.elements["f_nazwisko"];
  var kod = form.elements["f_kod"];
  var ulica = form.elements["f_ulica"];
  var miasto = form.elements["f_miasto"];
  var email = form.elements["f_email"];

  if (
    !checkAndFocus(imie, "Podaj imię!", isWhiteSpaceOrEmpty) ||
    !checkAndFocus(nazwisko, "Podaj nazwisko!", isWhiteSpaceOrEmpty) ||
    !checkAndFocus(kod, "Podaj kod pocztowy!", isWhiteSpaceOrEmpty) ||
    !checkAndFocus(ulica, "Podaj ulicę!", isWhiteSpaceOrEmpty) ||
    !checkAndFocus(miasto, "Podaj miasto!", isWhiteSpaceOrEmpty) ||
    !checkAndFocus(email, "Podaj właściwy e-mail", IsEmailInvalid)
  ) {
    return false;
  } else {
    return true;
  }
}

function showElement(e) {
  document.getElementById(e).style.visibility = "visible";
}

function hideElement(e) {
  document.getElementById(e).style.visibility = "hidden";
}

function alterRows(i, e) {
  if (e) {
    if (i % 2 == 1) {
      e.setAttribute("style", "background-color: Aqua;");
    }
    e = e.nextSibling;
    while (e && e.nodeType != 1) {
      e = e.nextSibling;
    }
    alterRows(++i, e);
  }
}

function nextNode(e) {
  while (e && e.nodeType != 1) {
    e = e.nextSibling;
  }
  return e;
}

function prevNode(e) {
  while (e && e.nodeType != 1) {
    e = e.previousSibling;
  }
  return e;
}

function swapRows(b) {
  let tab = prevNode(b.previousSibling);
  let tBody = nextNode(tab.firstChild);
  let lastNode = prevNode(tBody.lastChild);
  tBody.removeChild(lastNode);
  let firstNode = nextNode(tBody.firstChild);
  tBody.insertBefore(lastNode, firstNode);
}

function cnt(form, msg, maxSize) {
  if (form.value.length > maxSize)
    form.value = form.value.substring(0, maxSize);
  else msg.innerHTML = maxSize - form.value.length;
}

window.onload = function () {
  alterRows(1, document.getElementsByTagName("tr")[0]);
};
