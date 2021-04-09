// Timer Variables
var intervalName;
var timerCount = 0;
// API variables
var apiKey1 = "5grBGkT7";
var apiKey2 = "QZXnwbce";

var serverResponse;

// Function to check if there is already a timer running and start it
// Also send a command to start the study mode on the arduino
function startTimer(){
    if (timerCount != 0){
        clearInterval(intervalName);
        timerTest();
    } else { timerTest(); }
}

// Function to run the actual timer
function timerTest(){
    timerCount++;
    //Clear the end html element 
    document.getElementById("timerEnded").innerHTML = ""
    //Calculate the end time of the timer in Epoch (ms)
    var currentTime = new Date().getTime();
    var timerHours = document.getElementById("inputHour").value * 3600000;
    var timerMinutes = document.getElementById("inputMinutes").value * 60000;
    
    //Check if user entered a value or not
    if(timerHours == null || timerMinutes == null) {
        timerHours = 0;
        timerMinutes = 0;
    }

    var timerDuration = currentTime + parseInt(timerHours) + parseInt(timerMinutes);

    //Loop that will for the duration of the timer
    var changedIntervalName = false;
    var timerFunc = setInterval(function() {
        //Check if interval name has been stored
        if (changedIntervalName != true) {
            intervalName = timerFunc;
            changedIntervalName = true;
        }

        var now = new Date().getTime();
        var timeLeft = timerDuration - now;
        
        //Turning epoch time into a readable number
        var hours = Math.floor((timeLeft % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
        var minutes = Math.floor((timeLeft % (1000 * 60 * 60)) / (1000 * 60));
        var seconds = Math.floor((timeLeft % (1000 * 60)) / 1000);

        //Writing the values to the html and checking if timer is finished
        if (timeLeft < 0) {
            clearInterval(timerFunc);
            document.getElementById("timerOutputHours").innerHTML = ""
            document.getElementById("timerOutputMins").innerHTML = ""
            document.getElementById("timerOutputDots").innerHTML = ""
            document.getElementById("timerEnded").innerHTML = "Taak voltooid!"
        }
        else if (timeLeft > 3600000) {
            document.getElementById("timerOutputHours").innerHTML = hours + "h "
            document.getElementById("timerOutputMins").innerHTML = minutes + "m " 
        }
        else if (timeLeft < 60000) {
            document.getElementById("timerOutputHours").innerHTML = "";
            document.getElementById("timerOutputDots").innerHTML = ""
            document.getElementById("timerOutputMins").innerHTML = seconds + "s "
        }
        else {
            document.getElementById("timerOutputHours").innerHTML = minutes + "m "
            document.getElementById("timerOutputMins").innerHTML = seconds + "s " 
        }
    }, 1000)

    
}

function test() {
    receiveData(apiKey2);
}

function storeServer1Data() {
    serverResponse = this.responseText;
    console.log("Response from key: " + apiKey2);
    console.log("> " + serverResponse);
    document.getElementById("serverResponse").innerHTML = serverResponse;
}

function sendData(apiKey, value) {
    
}

// Wat is de geeft XMLHttpRequest precies terug
function receiveData(apiKey) {
    var requestString = "http://cmd.camp:12345/get/" + apiKey;
    console.log("Requesting data from: " + requestString);

    var server = new XMLHttpRequest();
    server.onload = storeServer1Data;
    server.open("GET", requestString);
    server.send();
}