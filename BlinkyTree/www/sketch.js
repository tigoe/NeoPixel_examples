/*
    sketch for the hue, saturation, a and value page
    created 15 Dec 2018
*/

var twinkleButton, resetButton, offButton, result;
var  intensitySlider, hueSlider, satSlider, hueSpan, intensitySpan, satSpan;

function setup() {
    // you're not doing any drawing, so no need for a canvas:
    noCanvas();

    // set up some buttons: a button  to generate a random twinkle:
    twinkleButton = createButton('Twinkle');
    twinkleButton.position(10, 65);
    twinkleButton.mousePressed(twinkle);

    // a button to reset the lights:
    resetButton = createButton('reset lights');
    resetButton.position(10, 95);
    resetButton.mousePressed(resetStrip);

    // a button to turn lights off:
    offButton = createButton('turn off lights');
    offButton.position(10, 125);
    offButton.mousePressed(turnoff);

    // a slider for the hue:
    hueSlider = createSlider(0, 359, 1);
    hueSpan = createSpan('hue: ' + hueSlider.value() + 'deg.');
    hueSpan.position(10, 155);
    hueSlider.position(220, 155);
    hueSlider.mouseReleased(changeHue);
    hueSlider.style('width', '200px');

    // a slider for the saturation:
    satSlider = createSlider(0, 100, 1);
    satSpan = createSpan('sat: ' + satSlider.value() + '%');
    satSpan.position(10, 185);
    satSlider.position(220, 185);
    satSlider.mouseReleased(changeSat);
    satSlider.style('width', '200px');

    // a slider for the intensity:
    intensitySlider = createSlider(0, 100, 1);
    intensitySpan = createSpan('intensity : '+ intensitySlider.value() +'%');
    intensitySpan.position(10, 215);
    intensitySlider.position(220, 215);
    intensitySlider.mouseReleased(changeIntensity);
    intensitySlider.style('width', '200px');
}

// a listener function for the twinkle button:
function twinkle() {
    result = httpGet('/mailbox/twinkle');
}

// a listener function for the reset button:
function resetStrip() {
    result = httpGet('/mailbox/on');
}

// a listener function for the off button:
function turnoff() {
    result = httpGet('/mailbox/off');
}

// a listener function for the hue slider:
function changeHue() {
    hueSpan.html('hue: ' + hueSlider.value());
    result = httpGet('/mailbox/h/' + hueSlider.value());
}

// a listener function for the saturation slider:
    function changeSat() {
    satSpan.html('saturation: ' + satSlider.value());
    result = httpGet('/mailbox/s/' + satSlider.value());
}

// a listener function for the intensity slider:
function changeIntensity() {
    //hue finished:
    intensitySpan.html('intensity: ' + valueSlider.value());
    result = httpGet('/mailbox/v/' + valueSlider.value());
}
