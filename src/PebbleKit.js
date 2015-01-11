var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

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

      var message = 'You need to take '+ amount+" of "+drug;

      var dictionary = {
        "KEY_MESSAGE": message,
        "KEY_TIME": 'time',
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

    getMedication();
    var looper = function(){
      getMedication();
      console.log('looped');
      setTimeout(looper,10000);
      };
    looper();
    }

);
