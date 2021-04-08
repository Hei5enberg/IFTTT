//Try a while loop

var timerName;
var timerCount = 0;

function startTimer() {
    console.log("poep")
    var currentDate = new Date().getTime();
    var timerDuration = currentDate + parseInt(document.getElementById("timerHours").value * 3600000) + parseInt(document.getElementById("timerMinutes").value * 60000);
    
    if (timerCount > 1) {
        console.log("more than 1 timer");
        clearInterval(timerName);
        timerCount = 0;
    } else {
        timerCount++;
        console.log("just runnning timer");
        var intervalName = timer(timerDuration);
        window.setInterval(intervalName, 1000);
    }
}

function timer(duration) {
    console.log("timer function start")
    // timerCount++;
    // console.log(timerCount);
    // var currentDate = new Date().getTime();
    // var timerDuration = currentDate + parseInt(document.getElementById("timerHours").value * 3600000) + parseInt(document.getElementById("timerMinutes").value * 60000);
    // console.log(timerDuration);

    // var timerFunc = setInterval(function() {
        // console.log("starting interval");
        var now = new Date().getTime();

        // var timeLeft = timerDuration - now;
        var timeLeft = duration - now;
        // console.log("time left")
        // console.log(timeLeft);

        var hours = Math.floor((timeLeft % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
        var minutes = Math.floor((timeLeft % (1000 * 60 * 60)) / (1000 * 60));
        var seconds = Math.floor((timeLeft % (1000 * 60)) / 1000);

        document.getElementById("hours").innerHTML = minutes + "m";
        document.getElementById("mins").innerHTML = seconds + "s";

        if (timeLeft < 0) {
            console.log("case 1");
            clearInterval(timerFunc);
            document.getElementById("hours").innerHTML = "done";
            document.getElementById("mins").innerHTML = "done";
        }
        
        // else if (timeLeft < 3600000) {
        //     console.log("case 2");
        //     document.getElementById("hours").innerHTML = minutes + "m";
        //     document.getElementById("mins").innerHTML = seconds + "s";
        // }
        // else {
        //     console.log("case 3");
        //     document.getElementById("hours").innerHTML = hours + "h";
        //     document.getElementById("mins").innerHTML = minutes + "m";
        // }
    // }(), 1000);

    // console.log(timerFunc);

    // timerName = timerFunc;
    console.log("timer function stop");
}

function blayt() {
    document.getElementById("hours").innerHTML = "kkr"
    document.getElementById("mins").innerHTML = "ja oke"
}
