var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function send_help() {
    var url = "http://cara.rowealex.com/create_alert?message=Help+I+Fell+And+My+Dog"
	xhrRequest(url, 'GET',
      function(responseText){}
    );
}

function locationSuccess(pos) {
  // Construct URLA
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude;

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // Temperature in Kelvin requires adjustment
      var temperature = Math.round(json.main.temp - 273.15);
      console.log("Temperature is " + temperature);

      // Conditions
      var conditions = json.weather[0].main;      
      console.log("Conditions are " + conditions);
      
      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_TEMPERATURE": temperature,
        "KEY_CONDITIONS": conditions
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Weather info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending weather info to Pebble!");
        }
      );
    }      
  );
}

function locationError(err) {
  console.log("Error requesting location!");
}

function getWeather() {
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

      if (json.status == 'failure') {
        return;
      }

      console.log("Medication is " + JSON.stringify(json));

      var amount = json.amount;
      console.log("Amount is : " + amount);

      var drug = json.name;
      console.log("Drug is " + drug);

      var time = json.time;      
      console.log("Time are " + time);

      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_DRUG": drug,
//         "KEY_TIME": time, 
//         "KEY_AMOUNT": amount
		
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Schedule info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending schedule info to Pebble!");
        }
      );
    }      
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");

    // Get the initial weather
    getMedication();
    var looper = function(){
      getMedication();
      console.log('looped');
      setTimeout(looper,10000);
      };
    looper();
    }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received:" + JSON.stringify(e.payload));
    if (e.payload[5] == 5){
       send_help();
    }else {
       getMedication();
    }

  }                     
);
