var intervalName;
var timerCount = 0;

function startTimer(){
    if (timerCount != 0){
        clearInterval(intervalName);
        timerTest();
    } else { timerTest(); }
}

function timerTest(){
    timerCount++;
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
            clearInterval(myfunc);
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