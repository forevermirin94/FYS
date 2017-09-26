<?php
	// de inlogcreds voor de database staan in phpconfig.php, voor veiligheid
	include_once 'phpconfig.php';
	session_start();
  
	// sanitize_string functie voor het verwijderen van special chars, anders wordt de database gek
	$username = filter_var($_POST['username'], FILTER_SANITIZE_STRING);
	$password = filter_var($_POST['password'], FILTER_SANITIZE_STRING);

	// sql info; opslaan in variable $sql
	$sql = new mysqli(HOST, USER, PASS, DATABASE);

	// query prepareren om te checken of de ingevoerde waarde overeen komt met een waarde in de database.. 
	// natuurlijk met md5 encryptie
	$stmt = $sql->prepare("SELECT id FROM Login WHERE Username = ? and Password = md5(?)");

	// de waarden uit $_post username en password toevoegen aan de vers geprepareerde query
	$stmt->bind_param('ss', $username, $password); 

	// query uitvoeren op de database engine
	$stmt->execute();

	// resultaat opslaan in zelfde variabele, deze hebben we toch niet meer nodig
	$stmt->store_result();

	if ($stmt->num_rows == 1) {
		// koppel de id van de ingevoerde gebruiker aan een variabele
		$stmt->bind_result($id); 
		$stmt->fetch();

		// tijd en datum van laatste login opslaan in de table
		// zelfde truc met het prepareren van een statement en aanvullen met bind_param
		$stmt = $sql->prepare("UPDATE Login SET last_login = NOW() WHERE id = ?");
		$stmt->bind_param('d', $id); 
		$stmt->execute();
	
		// instellen van sessievariabelen
		$_SESSION['username'] = $_POST['username'];
		$_SESSION['id'] = $id;
		$_SESSION['login'] = TRUE;

		// alles goedgegaan? we gaan door naar main.php, de hoofdpagina
		header('Location:../main.php');
	}
	
	else{
		// we komen hier alleen als de user niet bestaat; zet dit in de errors variabele
		$_SESSION['errors'] = array("Invalid Username or Password!");
		// en een redirect naar de inlogpagina
		header('Location:../index.php');
	}
?>
