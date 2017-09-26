<?php 
	include_once 'phpconfig.php'; //server connection info
	
	$mysqli = new mysqli(HOST, USER, PASS, DATABASE);
	
	$sql = "SELECT `Noodsituatie` FROM `Setting`";
	$result = $mysqli->query($sql);
	$noodsituatie = $result->fetch_assoc();
	
	if($noodsituatie[Noodsituatie] == 1) // is het een noodsituatie??
	{
		$status = '<a type="button" id="statusNood" onclick="resetNoodsituatie()"><p style="background: red;"><strong>Noodsituatie!</br>Klik hier!</strong></P></a>';
	} else { // geen noodsituatie
		$status = '<p><strong>Geen </br> noodsituatie</strong></P>';
	}
	
	// een script die het achtergrond en het tekst verandert
	// roept resetNoodsituatie.php aan
	echo ("
			<script>
				function resetNoodsituatie()
				{
					$.ajax({
					   type: 'POST',
					   url: 'php/resetNoodsituatie.php',
						success: function(data) {
							  $('.alert.gebouw').css('background', 'orange');
							  $('#statusNood').html('<p><strong>Noodsituatie </br>gereset</strong></P>');
						 }
					   
					   
					 });
				}
			</script>
					
					" . $status /* print het variabel met het html code erin beeschreven*/ . "
	");
?>
