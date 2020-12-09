const offOpacity = 0.2;
const onOpacity = 1.0;

function requestData(url) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {};
  xhttp.open("GET", url, true);
  xhttp.send();
}

function toggleLight(lightElement, lightColor) {
  let curOpacity = getComputedStyle(lightElement).getPropertyValue("opacity");
  let lightOn;
  if (curOpacity > 0.5) {
    lightOn = true;
  } else {
    lightOn = false;
  }

  if (lightOn) {
    lightElement.style.opacity = offOpacity;
    if (lightColor === "red") {
      redLightOff();
    } else if (lightColor === "yellow") {
      yellowLightOff();
    } else if (lightColor === "green") {
      greenLightOff();
    }
  }
  if (!lightOn) {
    lightElement.style.opacity = onOpacity;
    if (lightColor === "red") {
      redLightOn();
    } else if (lightColor === "yellow") {
      yellowLightOn();
    } else if (lightColor === "green") {
      greenLightOn();
    }
  }
}

// Red Light
function redLightOn() {
  let el = document.getElementsByClassName("red")[0];
  el.style.opacity = onOpacity;
  console.log("Red on");
  requestData("/relay1/on");
}

function redLightOff() {
  let el = document.getElementsByClassName("red")[0];
  el.style.opacity = offOpacity;
  console.log("Red off");
  requestData("/relay1/off");
}

// Yellow Light
function yellowLightOn() {
  let el = document.getElementsByClassName("yellow")[0];
  el.style.opacity = onOpacity;
  console.log("yellow on");
  requestData("/relay2/on");
}

function yellowLightOff() {
  let el = document.getElementsByClassName("yellow")[0];
  el.style.opacity = offOpacity;
  console.log("yellow off");
  requestData("/relay2/off");
}

// Green Light
function greenLightOn() {
  let el = document.getElementsByClassName("green")[0];
  el.style.opacity = onOpacity;
  console.log("green on");
  requestData("/relay3/on");
}

function greenLightOff() {
  let el = document.getElementsByClassName("green")[0];
  el.style.opacity = offOpacity;
  console.log("green off");
  requestData("/relay3/off");
}

function turnLightOn(el) {
  el.style.opacity = onOpacity;
}

function turnLightOff(el) {
  el.style.opacity = offOpacity;
}

function startPattern() {
  requestData("/pattern/start");
}

function stopPattern() {
  requestData("/pattern/stop");
}
