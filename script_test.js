function startTimer(){
    timerTest();
}

function timerTest(){
    //Calculate the end time of the timer in Epoch (ms)
    var currentTime = new Date().getTime();
    var timerHours = document.getElementById("inputHour").value * 3600000
    var timerMinutes = document.getElementById("inputMinutes").value * 60000
    
    //Check if user entered a value or not
    if(timerHours == null || timerMinutes == null) {
        timerHours = 0;
        timerMinutes = 0;
    }

    var timerDuration = currentTime + parseInt(timerHours) + parseInt(timerMinutes);

    //Loop that will for the duration of the timer
    var myfunc = setInterval(function() {
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
            document.getElementById("timerEnded").innerHTML = "TIME UP!!";
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