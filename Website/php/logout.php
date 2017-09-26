<?php
	//logout: open de session
	//daarna, unset de variabelen en destroy
	session_start();
	if (isset($_SESSION['username'])) {
		unset($_SESSION['username']);
		$_SESSION['errors'] = array("Uitgelogd!");
		session_destroy();
		header('Location:../index.php');
	} 
?>
