//Alle javascript die de website main.php nodig heeft
function resetDoor()
{
	$.ajax({
	   type: 'POST',
	   url: 'php/openDoor.php',
	 });
}

function kachelOn()
{
	$.get("php/curlFetchOn.php");
	
	 $('#statusNood').css("color", "green");
	 $('#statusNood').html('Kachel is aangezet');
	 return false;
}

function kachelOff()
{
	$.get("php/curlFetch.php");
	
	
	 $('#statusNood').css("color", "red");
	 $('#statusNood').html('Kachel is uitgezet');
	 return false;
}