function scrollWin() {
    window.scrollTo({ top: 947, behavior: 'smooth' });
}

function myFilter() {
  alert("Hello! What you see is all you get! There's only demo");
}

window.onscroll = function() {myFunction()};

var header = document.getElementById("myHeader");
var sticky = header.offsetTop;

function myFunction() {
  if (window.pageYOffset > sticky) {
    header.classList.add("sticky");
  } else {
    header.classList.remove("sticky");
  }
}
