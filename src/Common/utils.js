.pragma library

var TimeUtils = (function constructor () {
    var secondsInDate = 1;
    var minutesInDate = secondsInDate * 60;
    var hoursInDate   = minutesInDate * 60;

    function modifyDate (timeLeft, part, modulo) {
        var partValue = Math.floor(timeLeft / part);
        if (modulo) {
            partValue %= modulo;
        }

        timeLeft -= partValue;
        return partValue;
    }

    function calculateTimeLeft (timestamp) {
        return (timestamp - Date.now())/1000;
    }

    function calculateJobPercentage (millisecondsToEnd, baseTimeout) {
        return (baseTimeout - millisecondsToEnd) / baseTimeout;
    }

    function timeLeftToString(timeLeft) {
        var seconds = modifyDate(timeLeft, secondsInDate, 60);
        var minutes = modifyDate(timeLeft, minutesInDate, 60);
        var hours   = modifyDate(timeLeft, hoursInDate,   0 );

        return [ hours, minutes, seconds ]
            .map(function (seg) { return ("00" + seg).slice(-2); })
            .join(':');
    }

    return {
        calculateTimeLeft: calculateTimeLeft,
        calculateJobPercentage: calculateJobPercentage,
        timeLeftToString: timeLeftToString
    }

})()
