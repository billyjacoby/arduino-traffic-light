// console.log(JSON.stringify(patterns));

const offOpacity = 0.2;
const onOpacity = 1.0;

function toggleLight(lightElement) {
  let curOpacity = getComputedStyle(lightElement).getPropertyValue("opacity");
  let lightOn;
  if (curOpacity > 0.5) {
    lightOn = true;
  } else {
    lightOn = false;
  }

  if (lightOn) {
    lightElement.style.opacity = offOpacity;
  }
  if (!lightOn) {
    lightElement.style.opacity = onOpacity;
  }
}

function turnLightOn(el) {
  el.style.opacity = onOpacity;
}

function turnLightOff(el) {
  el.style.opacity = offOpacity;
}

function executeStateCall(state, el) {
  if (state) {
    turnLightOn(el[0]);
  } else {
    turnLightOff(el[0]);
  }
}

function setLightState(patternItem) {
  executeStateCall(
    patternItem.state.red,
    document.getElementsByClassName("red")
  );
  executeStateCall(
    patternItem.state.yellow,
    document.getElementsByClassName("yellow")
  );
  executeStateCall(
    patternItem.state.green,
    document.getElementsByClassName("green")
  );
}

function sleep(delay) {
  return new Promise((resolve) => setTimeout(resolve, delay));
}

let currentRunningPattern;

function runPattern(patternSlug) {
  let curPattern = patterns.filter((item) => item.slug === patternSlug)[0];
  // if (curPattern.continuous) {
  //   let totalDuration = curPattern.pattern.reduce((acc, cur) => acc + cur, 0);
  //   // do forever
  // } else {
  //   // do once
  // }
  curPattern.pattern.forEach((item, index) => {
    let timeout;
    if (index === 0) {
      timeout = 0;
    } else {
      timeout = item.duration;
    }

    setTimeout(() => {
      setLightState(item);
    }, timeout);
  });
}

function addEventListenerList(list, event, fn) {
  for (var i = 0, len = list.length; i < len; i++) {
    list[i].addEventListener(event, fn, false);
  }
}
function recordPattern() {
  addEventListenerList(document.getElementsByClassName("light"), "click", () =>
    console.log("clicked: ", event.target)
  );
}
