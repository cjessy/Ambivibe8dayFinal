// var ACTIVITIES = ["STATIONARY", "RUNNING", "DRIVING", "WALKING", "CYCLING", "NIL"];
// var USER_RESPONSES = ["FELT", "NOT FELT", "NO TIME", "NO RESPONSE"];
// var CERTAINTY = ["NIL", "VERY CERTAIN", "SOMEWHAT CERTAIN", "SOMEWHAT UNCERTAIN", "VERY UNCERTAIN"];

function pad(n, width, z) {
  z = z || '0';
  n = n + '';
  return n.length >= width ? n : new Array(width - n.length + 1).join(z) + n;
}

Pebble.addEventListener('ready',
  function(e) {
    console.log('JavaScript app ready and running!');
  }
);

Pebble.addEventListener('appmessage',
  function(e) {
    var message = e.payload;
    var key = Object.keys(message)[0];
    console.log("key: " + key + "\n");
    if (key == "response"){
      console.log('Received response to send to server');
      var fields = message.response; //byte array
      console.log("fields: " + fields + "\n");
      var req = new XMLHttpRequest();
      var date = new Date();
      var year = date.getFullYear();
      var url = "http://ambivibe.thomaspietrzak.com?partnum=" + fields[0] + "&" +   //participant number
                    "resp=" + fields[1] + "&" +   //user reponse       
                    "presig=" + (fields[0] % 2) + "&" +     //presignal (0 = no presignal)
                    "icon=" + fields[2] + "&" +           //icon sent
                    "guess=" + fields[3] + "&" +                   //icon guessed    
                    "act=" + fields[4] + "&" +                 //user activity (self report)
                    "cert=" + fields[5] + "&" +        //user certainty 
                    "vib-t=" + pad(fields[6], 2)+ "-" + pad(fields[7],2) + "-" + pad(fields [8], 2) + "&" +   //time vibration sent
                    "resp-t=" + pad(fields[9], 2)+ "-" + pad(fields[10],2) + "-" + pad(fields[11], 2) + "&" +  //time response sent
                    "date=" + year + "-"  + pad(fields[12], 2)+ "-" + pad(fields[13], 2);         //date
      console.log(url);
      req.open("GET", url, true);
      req.onload = function(e) {
         if (req.status == 200) {
             console.log('response send success');
         }
         else {
           console.log('response send fail');
         }
      };
  
      req.onerror = function(e) {
         console.log('response send fail: '+ JSON.stringify(e));
       };
      req.send();
      } else if (key == "ping") {
        var pingReq = new XMLHttpRequest();
        var participant = message.ping;
        console.log("ping" + participant + "\n");
        var pingUrl =  "http://ambivibe.thomaspietrzak.com/ping.php?partnum=" + participant;
        console.log(pingUrl);
        pingReq.open("GET", pingUrl, true);
      }    
  } 

);