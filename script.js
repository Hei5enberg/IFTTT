var timerName;

function startTimer() {
    clearInterval(timerName);
    timer();
}

function timer() {
    var currentDate = new Date().getTime();
    var timerDuration = currentDate + parseInt(document.getElementById("timerHours").value * 3600000) + parseInt(document.getElementById("timerMinutes").value * 60000);

    var timerFunc = setInterval(function() {
        var now = new Date().getTime();

        var timeLeft = timerDuration - now;

        var hours = Math.floor((timeLeft % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
        var minutes = Math.floor((timeLeft % (1000 * 60 * 60)) / (1000 * 60));
        var seconds = Math.floor((timeLeft % (1000 * 60)) / 1000);

        if (timeLeft < 0) {
            clearInterval(timerFunc);
            timerCount = 0;
            document.getElementById("hours").innerHTML = "done";
            document.getElementById("mins").innerHTML = "done";
        }
        else if (timeLeft < 3600000) {
            document.getElementById("hours").innerHTML = minutes + "m";
            document.getElementById("mins").innerHTML = seconds + "s";
        }
        else {
            document.getElementById("hours").innerHTML = hours + "h";
            document.getElementById("mins").innerHTML = minutes + "m";
        }
    }, 1000);

    timerName = timerFunc;
}
