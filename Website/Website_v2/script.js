// Timer Variables
var intervalName;
var timerCount = 0;
// API variables
var apiKey1 = "5grBGkT7";
var apiKey2 = "QZXnwbce";


var serverResponse;
var userUsingCase = 0;

// Function to check if there is already a timer running and start it
// Also send a command to start the study mode on the arduino
function receiveData(apiKey) {
    var requestString = "http://cmd.camp:12345/get/" + apiKey;
    console.log("Requesting data from: " + requestString);

    var server = new XMLHttpRequest();

    // REMOVE IF STATEMENT IN NON DEV VERSION
    if (apiKey == apiKey1){
        server.onload = procesServerArduinoData;
    } else { console.log("ERROR: Nowhere to send apiKey2 data") }
    
    server.open("GET", requestString);
    server.send();
}

function runTimer(){
    timerCount++;
    // Clear the end html element 
    document.getElementById("timerEnded").innerHTML = ""
    // Calculate the end time of the timer in Epoch (ms)
    var currentTime = new Date().getTime();
    var timerHours = document.getElementById("inputHour").value * 3600000;
    var timerMinutes = document.getElementById("inputMinutes").value * 60000;
    
    // Check if user entered a value or not
    if(timerHours == null || timerMinutes == null) {
        timerHours = 0;
        timerMinutes = 0;
    }

    var timerDuration = currentTime + parseInt(timerHours) + parseInt(timerMinutes);

    // Loop that will for the duration of the timer
    var changedIntervalName = false;
    var timerFunc = setInterval(function() {
        // Check if interval name has been stored
        if (changedIntervalName != true) {
            intervalName = timerFunc;
            changedIntervalName = true;
        }

        var now = new Date().getTime();
        var timeLeft = timerDuration - now;
        var timeLeftSeconds = Math.round(timeLeft / 1000);
        sendData(apiKey2, timeLeftSeconds)
        
        // Turning epoch time into a readable number
        var hours = Math.floor((timeLeft % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
        var minutes = Math.floor((timeLeft % (1000 * 60 * 60)) / (1000 * 60));
        var seconds = Math.floor((timeLeft % (1000 * 60)) / 1000);

        // Changing HTML elements
        receiveData(apiKey1);
        if (userUsingCase == 1) {
            document.getElementById("user_interrupt_alert").style.display = "flex"
            document.getElementById("user_interrupt_text").innerHTML = "GA VAN JE TELEFOON AF!"
            document.getElementById("user_interrupt_alert").style.backgroundColor = "red"
        } 
        else if (userUsingCase == 0) {
            document.getElementById("user_interrupt_alert").style.display = "none"
            document.getElementById("user_interrupt_text").innerHTML = ""
            document.getElementById("user_interrupt_alert").style.backgroundColor = "white"
        } else { console.log("ERROR: userUsingCase variable broken"); }

        if (timeLeft < 0) {
            clearInterval(timerFunc);
            sendData(apiKey2, 0);
            document.getElementById("timer-running").style.display = "none"
            document.getElementById("timer-finished").style.display = "block"

            document.getElementById("timerOutputHours").innerHTML = ""
            document.getElementById("timerOutputMins").innerHTML = ""
            document.getElementById("timerEnded").innerHTML = "Je hebt het vol gehouden!"
            setTimeout(() => {
                document.getElementById("timer-finished").style.display="none";
                document.getElementById("timer").style.display = "block";
                document.getElementById("timer-running").style.display = "none"; 
        }, 7000)
        }
        else if (timeLeft < 60000) {
            document.getElementById("timerOutputHours").innerHTML = "";
            document.getElementById("timerOutputMins").innerHTML = seconds + "s "
        }
        else if (timeLeft > 3600000) {
            document.getElementById("timerOutputHours").innerHTML = hours + "h "
            document.getElementById("timerOutputMins").innerHTML = minutes + "m " 
        }
        else {
            document.getElementById("timerOutputHours").innerHTML = minutes + "m "
            document.getElementById("timerOutputMins").innerHTML = seconds + "s " 
        }
    }, 1000)

    
}

function sendData(apiKey, value) {
    var requestString = "http://cmd.camp:12345/send/" + apiKey + "/" + value;
    var server = new XMLHttpRequest();
    server.open("GET", requestString);
    server.send();
}

//document.getElementById("timerRunning").style.display = "none";
function startTimer(){
    document.getElementById("timer").style.display = "none";
    document.getElementById("timer-running").style.display = "block";
    if (timerCount != 0){
        clearInterval(intervalName);
        runTimer();
    } else { runTimer(); }
}

function stopTimer() {
    document.getElementById("timer").style.display = "block";
    document.getElementById("timer-running").style.display = "none";
    clearInterval(intervalName);
}

function procesServerArduinoData() {
    serverResponse = this.responseText;
    console.log("Response from key: " + apiKey1);
    console.log("> " + serverResponse);
    userUsingCase = serverResponse;
}