<?php
function generate_pdf()
{
	$mpath = drupal_get_path('module', 'circuit_simulation');
	require($mpath.'/pdf/fpdf/fpdf.php');
	require($mpath.'/pdf/phpqrcode/qrlib.php');
	global $user;
	$x                 = $user->uid;
	$proposal_id       = arg(3);
	$query3 = db_query("SELECT * FROM esim_circuit_simulation_proposal WHERE approval_status=3 AND uid= :uid AND id=:proposal_id", array(
		':uid' => $user->uid,
		':proposal_id'=>$proposal_id
	));
	$data3             = $query3->fetchObject();
	/*$query3 = db_query("SELECT * FROM dwsim_flowsheet_proposal WHERE approval_status=3 AND uid= :uid", array(
		':uid' => $user->uid
	));
	$data3             = $query3->fetchObject();*/
	if($data3){
	if($data3->uid != $x){
		drupal_set_message('Certificate is not available','error');
		return;
	}
	}
	$gender            = array(
		'salutation' => 'Mr. /Ms.',
		'gender' => 'He/She'
	);
	if ($data3->gender) {
		if ($data3->gender == 'M') {
			$gender = array(
				'salutation' => 'Mr.',
				'gender' => 'He'
			);
		} //$data3->gender == 'M'
		else {
			$gender = array(
				'salutation' => 'Ms.',
				'gender' => 'She'
			);
		}
	} //$data3->gender
	$pdf = new FPDF('L', 'mm', 'Letter');
	if (!$pdf) {
		echo "Error!";
	} //!$pdf
	$pdf->AddPage();
	$image_bg = $mpath . "/pdf/images/bg_cert.png";
	$pdf->Image($image_bg, 0, 0, $pdf->GetPageWidth(), $pdf->GetPageHeight());
	//$pdf->Rect(5, 5, 267, 207, 'D');
	$pdf->SetMargins(18, 1, 18);
	//$pdf->Line(7.0, 7.0, 270.0, 7.0);
	//$pdf->Line(7.0, 7.0, 7.0, 210.0);
	//$pdf->Line(270.0, 210.0, 270.0, 7.0);
	//$pdf->Line(7.0, 210.0, 270.0, 210.0);
	$path   = drupal_get_path('module', 'circuit_simulation');
	//$image1 = $mpath . "/pdf/images/dwsim_logo.png";
	$pdf->Ln(30);
	//$pdf->Cell(200, 8, $pdf->Image($image1, 105, 15, 0, 28), 0, 1, 'C');
	//$pdf->Ln(20);
	
	//$pdf->SetTextColor(139, 69, 19);
	//$pdf->Cell(240, 8, 'Certificate of Participation', '0', 1, 'C');
	//$pdf->Ln(26);
	$pdf->SetFont('Times', 'I', 16);
	$pdf->SetTextColor(0, 0, 0);
	$pdf->Cell(320, 10, 'This is to certify that', '0', '1', 'C');
	$pdf->Ln(-4);
	$pdf->SetFont('Times', 'I', 20);
	//$pdf->SetFont('Arial', 'BI', 25);
	$pdf->SetTextColor(129, 80, 47);
	$pdf->Cell(320, 12, $data3->contributor_name, '0', '1', 'C');
	$pdf->Ln(0);
	$pdf->SetFont('Times', 'I', 16);
	if (strtolower($data3->branch) != "others") {
		$pdf->SetTextColor(0, 0, 0);
		//$pdf->Cell(240, 8, 'from ' . $data3->university . ' has successfully', '0', '1', 'C');
		$pdf->MultiCell(320, 8, 'from ' . $data3->university, '0','C');
		$pdf->Ln(0);
		$pdf->Cell(320, 8, 'has successfully completed the circuit simulation of', '0', '1', 'C');
		$pdf->Ln(0);
		//$pdf->Cell(240, 8, 'He/she has simulated a circuit titled ', '0', '1', 'C');
		//$pdf->Ln(0);
		$pdf->SetTextColor(129, 80, 47);
		$pdf->SetFont('Times','I',15);
		$pdf->Cell(320, 12, $data3->project_title, '0', '1', 'C');
		$pdf->SetTextColor(0, 0, 0);
		$pdf->Ln(0);
		//$txt= $pdf->WriteHTML("<span>under</span>");
		$pdf->SetFont('Times', 'I', 16);
		$pdf->Cell(320, 8, 'under eSim Circuit Simulation Project', '0', '1', 'C');
		$pdf->Ln(0);
	} //strtolower($data3->branch) != "others"
	else {
		$pdf->SetTextColor(0, 0, 0);
		$pdf->Cell(240, 8, 'from ' . $data3->university . ' college', '0', '1', 'C');
		$pdf->Ln(0);
		$pdf->Cell(240, 8, 'has successfully completed the circuit of', '0', '1', 'C');
		$pdf->Ln(0);
		$pdf->SetTextColor(139, 69, 19);
		$pdf->Cell(320, 12, $data3->project_title, '0', '1', 'C');
		$pdf->SetTextColor(0, 0, 0);
		$pdf->Ln(0);
		$pdf->SetFont('Times', '', 16);
		$pdf->Cell(320, 8, ' under eSim Circuit Simulation Project', '0', '1', 'C');
		//$pdf->Cell(240, 8, 'He/she has coded ' . $number_of_example . ' solved examples using DWSIM from the', '0', '1', 'C');
		//$pdf->Ln(0);
		//$pdf->Cell(240, 8, 'Book: ' . $data2->book . ', Author: ' . $data2->author . '.', '0', '1', 'C');
		//$pdf->Ln(0);
	}
	$proposal_get_id = 0;
	$UniqueString    = "";
	$tempDir         = $path . "/pdf/temp_prcode/";
	$query           = db_select('esim_circuit_simulation_qr_code');
	$query->fields('esim_circuit_simulation_qr_code');
	$query->condition('proposal_id', $proposal_id);
	$result          = $query->execute();
	$data            = $result->fetchObject();
	$DBString        = $data->qr_code;
	$proposal_get_id = $data->proposal_id;
	if ($DBString == "" || $DBString == "null") {
		$UniqueString = generateRandomString();
		$query        = "
				INSERT INTO esim_circuit_simulation_qr_code
				(proposal_id,qr_code)
				VALUES
				(:proposal_id,:qr_code)
				";
		$args         = array(
			":proposal_id" => $proposal_id,
			":qr_code" => $UniqueString
		);
		$result       = db_query($query, $args, array(
			'return' => Database::RETURN_INSERT_ID
		));
	} //$DBString == "" || $DBString == "null"
	else {
		$UniqueString = $DBString;
	}
	$codeContents        = "https://esim.fossee.in/circuit-simulation-project/certificates/verify/" . $UniqueString;
	$fileName            = 'generated_qrcode.png';
	$pngAbsoluteFilePath = $tempDir . $fileName;
	$urlRelativeFilePath = $path . "/pdf/temp_prcode/" . $fileName;
	QRcode::png($codeContents, $pngAbsoluteFilePath);
	$pdf->SetY(85);
	$pdf->SetX(320);
	$pdf->Ln(10);
	$sign1 = $path . "/pdf/images/sign1.png";
	$sign2 = $path . "/pdf/images/sign2.png";
	$pdf->Image($sign1, $pdf->GetX()+60, $pdf->GetY() + 45, 85, 0);
	$pdf->Image($sign2, $pdf->GetX()+160, $pdf->GetY() + 45, 85, 0);
	$pdf->Image($pngAbsoluteFilePath, $pdf->GetX()+15, $pdf->GetY()+70, 30, 0);
	$fossee = $path . "/pdf/images/fossee.png";
	$mhrd = $path . "/pdf/images/mhrd.png"; 
	$pdf->Image($fossee, $pdf->GetX()+80, $pdf->GetY()+80, 50, 0);
	$pdf->Image($mhrd, $pdf->GetX()+180, $pdf->GetY()+80, 40, 0);
	//$pdf->SetX(29);
	//$pdf->SetY(-50);
	$pdf->Ln(2);
	$ftr_line = $path . "/pdf/images/ftr_line.png"; 
	$pdf->Image($ftr_line, $pdf->GetX()+50, $pdf->GetY()+102, 150, 0);
	$pdf->SetFont('Times', 'I', 15);
	$pdf->SetLeftMargin(40);
	$pdf->GetY()+60;
	$pdf->Ln(62);
	$pdf->Cell(320, 8, $UniqueString, '0', '1', 'L');
	//$pdf->Ln(6);
	$pdf->SetFont('Arial', 'I', 8);
	$pdf->SetTextColor(0, 0, 0);
	$filename = str_replace(' ', '-', $data3->contributor_name) . '-eSim-Circuit-Simulation-Certificate.pdf';
	$file     = $path . '/pdf/temp_certificate/' . $proposal_id . '_' . $filename;
	$pdf->Output($file, 'F');
	ob_clean();
	header("Pragma: public");
	header("Expires: 0");
	header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
	header("Cache-Control: public");
	header("Content-Description: File Transfer");
	header("Content-Type: application/pdf");
	header("Content-Disposition: attachment; filename=" . $filename);
	header("Content-Length: " . filesize($file));
	header("Content-Transfer-Encoding: binary");
	header("Expires: 0");
	header("Pragma: no-cache");
	flush();
	$fp = fopen($file, "r");
	while (!feof($fp)) {
		echo fread($fp, filesize($file));
		flush();
	} //!feof($fp)
	ob_end_flush();
	ob_clean();
	fclose($fp);
	unlink($file);
	//drupal_goto('flowsheeting-project/certificate');
	return;
}
function generateRandomString($length = 5)
{
	$characters       = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
	$charactersLength = strlen($characters);
	$randomString     = '';
	for ($i = 0; $i < $length; $i++) {
		$randomString .= $characters[rand(0, $charactersLength - 1)];
	} //$i = 0; $i < $length; $i++
	return $randomString;
}
function generate_copyright_form_pdf()
{
	$mpath = drupal_get_path('module', 'circuit_simulation');
	require($mpath . '/pdf/fpdf/fpdf.php');
	global $user;
	$x           = $user->uid;
	$proposal_id = arg(3);
	$query2 = db_query("SELECT id FROM esim_circuit_simulation_proposal WHERE approval_status=3 AND uid= :uid", array(
		':uid' => $user->uid
	));
	$data2             = $query2->fetchObject();
	$query3 = db_query("SELECT * FROM esim_circuit_simulation_proposal WHERE approval_status=3 AND uid= :uid", array(
		':uid' => $user->uid
	));
	$data3       = $query3->fetchObject();
	$gender      = array(
		'salutation' => 'Mr. /Ms.',
		'gender' => 'He/She'
	);
	if ($data3->gender) {
		if ($data3->gender == 'M') {
			$gender = array(
				'salutation' => 'Mr.',
				'gender' => 'He'
			);
		} //$data3->gender == 'M'
		else {
			$gender = array(
				'salutation' => 'Ms.',
				'gender' => 'She'
			);
		}
	} //$data3->gender
	$pdf = new FPDF('P', 'mm', 'Letter');
	if (!$pdf) {
		echo "Error!";
	} //!$pdf
	$pdf->AddPage();
	$path = drupal_get_path('module', 'circuit_simulation');
	$pdf->SetFont('Arial', 'B', 25);
	$pdf->Ln(30);
	$pdf->Cell(200, 8, 'Copyright Transfer Form', 0, 1, 'C');
	$pdf->Ln(20);
	$pdf->SetFont('Arial', '', 12);
	$pdf->MultiCell(200, 8, 'I hereby transfer the copyrights of the DWSIM Flowsheeting Project for ' . $data2->project_title . ' to FOSSEE Project, IIT Bombay.', 0);
	$pdf->Ln(10);
	$pdf->MultiCell(200, 8, 'I  understand  that  the  FOSSEE  project  will  release  the  Flowsheet  under  the Creative Commons (CC) license.');
	$pdf->SetX(75);
	$pdf->SetFont('', 'U');
	$pdf->SetTextColor(0, 0, 255);
	$pdf->SetFont('', '');
	$pdf->SetTextColor(0, 0, 0);
	$pdf->SetFont('', '');
	$pdf->SetTextColor(0, 0, 0);
	$pdf->SetY(-10);
	$pdf->SetX(209);
	$cur_date = date('jS F, Y');
	$pdf->SetY(140);
	$pdf->SetFont('', '');
	$pdf->Ln(0);
	$pdf->Cell(200, 0, ' Date: ' . $cur_date . '', 0, 1, 'L');
	$pdf->Cell(200, 20, ' Place: _________________', 0, 1, 'L');
	$pdf->SetY(140);
	$pdf->SetX(120);
	$pdf->Cell(180, 0, 'Signature: _________________', 0, 1, 'L');
	$pdf->SetY(144);
	$pdf->SetX(120);
	$pdf->multicell(140, 14, 'Name: ' . $gender['salutation'] . ' ' . $data3->full_name . '', 0, '');
	$filename = str_replace(' ', '-', $data3->full_name) . '-eSim-Circuit-Simulation-copyright-form.pdf';
	$file     = $path . '/pdf/temp_certificate/' . $proposal_id . '_' . $filename;
	$pdf->Output($file, 'F');
	header("Content-Type: application/octet-stream");
	header("Content-Disposition: attachment; filename=" . $filename);
	header("Content-Type: application/octet-stream");
	header("Content-Type: application/download");
	header("Content-Description: File Transfer");
	header("Content-Length: " . filesize($file));
	flush();
	$fp = fopen($file, "r");
	while (!feof($fp)) {
		echo fread($fp, 65536);
		flush();
	} //!feof($fp)
	fclose($fp);
	unlink($file);
	drupal_goto('Summer_Internship_Forms/forms');
	return;
}
function generate_undertaking_form_pdf()
{
	$mpath = drupal_get_path('module', 'circuit_simulation');
	require($mpath . '/pdf/fpdf/fpdf.php');
	global $user;
	$x           = $user->uid;
	$proposal_id = arg(3);
	$query2 = db_query("SELECT id FROM esim_circuit_simulation_proposal WHERE approval_status=3 AND uid= :uid", array(
		':uid' => $user->uid
	));
	$data2             = $query2->fetchObject();
	$query3 = db_query("SELECT * FROM esim_circuit_simulation_proposal WHERE approval_status=3 AND uid= :uid", array(
		':uid' => $user->uid
	));
	$data3       = $query3->fetchObject();
	$gender      = array(
		'salutation' => 'Mr. /Ms.',
		'gender' => 'He/She'
	);
	if ($data3->gender) {
		if ($data3->gender == 'M') {
			$gender = array(
				'salutation' => 'Mr.',
				'gender' => 'He'
			);
		} //$data3->gender == 'M'
		else {
			$gender = array(
				'salutation' => 'Ms.',
				'gender' => 'She'
			);
		}
	} //$data3->gender
	$pdf = new FPDF('P', 'mm', 'Letter');
	if (!$pdf) {
		echo "Error!";
	} //!$pdf
	$pdf->AddPage();
	$path = drupal_get_path('module', 'circuit_simulation');
	$pdf->SetFont('Arial', 'B', 25);
	$pdf->Ln(30);
	$pdf->Cell(200, 8, 'Undertaking Form', 0, 1, 'C');
	$pdf->Ln(0);
	$pdf->SetFont('Arial', 'B', 10);
	$pdf->Cell(200, 8, '(To be signed by college teacher)', 0, 1, 'C');
	$pdf->Ln(20);
	$pdf->SetFont('Arial', '', 12);
	$pdf->MultiCell(200, 8, 'I hereby certify that all the codes written by ' . $gender['salutation'] . ' ' . $data3->full_name . ' under the DWSIM Textbook Companion Project for the book ' . $data2->book . ' ( Author: ' . $data2->author . ', Edition: ' . $data2->edition . ', Publisher: ' . $data2->publisher . ', Year: ' . $data2->year . ') are correctly reproducing the results given in the aforementioned book.', 0);
	$pdf->Ln(10);
	$pdf->MultiCell(200, 8, 'I understand that the DWSIM Textbook Companion created is a part of FOSSEE project, IIT Bombay, and is sponsored by the National Mission on Education through Information and Communication Technology (NMEICT), under MHRD, Govt. of India. The project requires that the textbook companion is made available for public access as an open source document. Hence I undertake that this DWSIM Textbook Companion can be made public along with the information that I have certified all the codes as giving the correct answer.');
	$pdf->SetX(75);
	$pdf->SetFont('', 'U');
	$pdf->SetTextColor(0, 0, 255);
	$pdf->SetFont('', '');
	$pdf->SetTextColor(0, 0, 0);
	$pdf->SetFont('', '');
	$pdf->SetTextColor(0, 0, 0);
	$pdf->SetY(-10);
	$pdf->SetX(209);
	$cur_date = date('jS F, Y');
	$pdf->SetY(180);
	$pdf->SetFont('', '');
	$pdf->Ln(0);
	$pdf->Cell(200, 0, ' Date: ' . $cur_date . '', 0, 1, 'L');
	$pdf->Cell(200, 20, ' Place: _________________', 0, 1, 'L');
	$pdf->SetY(180);
	$pdf->SetX(120);
	$pdf->Cell(140, 0, 'Signature: _________________', 0, 1, 'L');
	$pdf->SetX(120);
	$pdf->multicell(140, 14, 'Name: ' . '____________________', 0, '');
	$pdf->SetX(120);
	$pdf->multicell(140, 14, 'Designation: ' . '______________', 0, '');
	$filename = str_replace(' ', '-', $data3->full_name) . '-eSim-Circuit-Simulation-undertaking-form.pdf';
	$file     = $path . '/pdf/temp_certificate/' . $proposal_id . '_' . $filename;
	$pdf->Output($file, 'F');
	header("Content-Type: application/octet-stream");
	header("Content-Disposition: attachment; filename=" . $filename);
	header("Content-Type: application/octet-stream");
	header("Content-Type: application/download");
	header("Content-Description: File Transfer");
	header("Content-Length: " . filesize($file));
	flush();
	$fp = fopen($file, "r");
	while (!feof($fp)) {
		echo fread($fp, 65536);
		flush();
	} //!feof($fp)
	fclose($fp);
	unlink($file);
	drupal_goto('Summer_Internship_Forms/forms');
	return;
}
