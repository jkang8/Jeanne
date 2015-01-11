var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function send_help() {
    var url = "http://cara.rowealex.com/create_alert?message=I+Need+Help"
	xhrRequest(url, 'GET',
      function(responseText){}
    );
}


function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

var timestamps = [];

function getMedication() {
  // Construct URL
  var url = "http://cara.rowealex.com/today";

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      console.log("Medication is " + JSON.stringify(json));
      if (json.status == 'failure') {
        return;
      }


      var amount = json.amount;
      console.log("Amount is : " + amount);

      var drug = json.name;
      console.log("Drug is " + drug);

      var time = json.time;      
      console.log("Time are " + time);

      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_DRUG": drug,
        "KEY_TIME": time, 
        "KEY_AMOUNT": amount
		
      };

      console.log('list'+ timestamps)
      console.log('timetammp: '+time)


      if (timestamps.indexOf(time) >= 0) {
        return;
      }
      
      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Schedule info sent to Pebble successfully!");
          timestamps.push(time);
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

    var looper = function() {
      getMedication();
      setTimeout(looper, 10000);
      console.log('Looped');
    };

    looper();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received:" + e.payload);
	if(e.payload[5]==5){
	   send_help();
	} else {
	   getMedication();
	}
  }                     
);
