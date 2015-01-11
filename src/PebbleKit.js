var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

// Send patients location to the caregiver upon request
function locationSuccess(pos) {
  // Construct URL
  var url = "http://cara.rowealex.com/location?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude;

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // no response expected
    }      
  );
}

function locationError(err) {
  console.log("Error requesting location!");
}

function sendLocation() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

function getMedication() {
  // Construct URL
  var url = "http://cara.rowealex.com/today";

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);
      console.log("Medication is " + JSON.stringify(json));
    }      
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");
    getMedication();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    getMedication();
  }                     
);
