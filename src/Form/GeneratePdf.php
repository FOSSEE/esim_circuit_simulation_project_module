<?php

/**
 * @file
 * Contains \Drupal\circuit_simulation\Form\GeneratePdf.
 */

namespace Drupal\circuit_simulation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;

class GeneratePdf extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'generate_pdf';
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $mpath = drupal_get_path('module', 'circuit_simulation');
    require($mpath . '/pdf/fpdf/fpdf.php');
    require($mpath . '/pdf/phpqrcode/qrlib.php');
    $user = \Drupal::currentUser();
    $x = $user->uid;
    $proposal_id = arg(3);
    $query3 = \Drupal::database()->query("SELECT * FROM esim_circuit_simulation_proposal WHERE approval_status=3 AND uid= :uid AND id=:proposal_id", [
      ':uid' => $user->uid,
      ':proposal_id' => $proposal_id,
    ]);
    $data3 = $query3->fetchObject();
    /*$query3 = db_query("SELECT * FROM dwsim_flowsheet_proposal WHERE approval_status=3 AND uid= :uid", array(
		':uid' => $user->uid
	));
	$data3             = $query3->fetchObject();*/
    if ($data3) {
      if ($data3->uid != $x) {
        \Drupal::messenger()->addError('Certificate is not available');
        return;
      }
    }
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
    $path = drupal_get_path('module', 'circuit_simulation');
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
      $pdf->MultiCell(320, 8, 'from ' . $data3->university, '0', 'C');
      $pdf->Ln(0);
      $pdf->Cell(320, 8, 'has successfully completed the circuit simulation of', '0', '1', 'C');
      $pdf->Ln(0);
      //$pdf->Cell(240, 8, 'He/she has simulated a circuit titled ', '0', '1', 'C');
      //$pdf->Ln(0);
      $pdf->SetTextColor(129, 80, 47);
      $pdf->SetFont('Times', 'I', 15);
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
    $UniqueString = "";
    $tempDir = $path . "/pdf/temp_prcode/";
    $query = \Drupal::database()->select('esim_circuit_simulation_qr_code');
    $query->fields('esim_circuit_simulation_qr_code');
    $query->condition('proposal_id', $proposal_id);
    $result = $query->execute();
    $data = $result->fetchObject();
    $DBString = $data->qr_code;
    $proposal_get_id = $data->proposal_id;
    if ($DBString == "" || $DBString == "null") {
      $UniqueString = generateRandomString();
      $query = "
				INSERT INTO esim_circuit_simulation_qr_code
				(proposal_id,qr_code)
				VALUES
				(:proposal_id,:qr_code)
				";
      $args = [
        ":proposal_id" => $proposal_id,
        ":qr_code" => $UniqueString,
      ];
      $result = \Drupal::database()->query($query, $args, $query);
    } //$DBString == "" || $DBString == "null"
    else {
      $UniqueString = $DBString;
    }
    $codeContents = "https://esim.fossee.in/circuit-simulation-project/certificates/verify/" . $UniqueString;
    $fileName = 'generated_qrcode.png';
    $pngAbsoluteFilePath = $tempDir . $fileName;
    $urlRelativeFilePath = $path . "/pdf/temp_prcode/" . $fileName;
    QRcode::png($codeContents, $pngAbsoluteFilePath);
    $pdf->SetY(85);
    $pdf->SetX(320);
    $pdf->Ln(10);
    $sign1 = $path . "/pdf/images/sign1.png";
    //$sign2 = $path . "/pdf/images/sign2.png";
    $pdf->Image($sign1, $pdf->GetX() + 60, $pdf->GetY() + 45, 85, 0);
    //$pdf->Image($sign2, $pdf->GetX()+160, $pdf->GetY() + 45, 85, 0);
    $pdf->Image($pngAbsoluteFilePath, $pdf->GetX() + 15, $pdf->GetY() + 70, 30, 0);
    $fossee = $path . "/pdf/images/fossee.png";
    $mhrd = $path . "/pdf/images/mhrd.png";
    $pdf->Image($fossee, $pdf->GetX() + 80, $pdf->GetY() + 80, 50, 0);
    $pdf->Image($mhrd, $pdf->GetX() + 180, $pdf->GetY() + 80, 40, 0);
    //$pdf->SetX(29);
    //$pdf->SetY(-50);
    $pdf->Ln(2);
    $ftr_line = $path . "/pdf/images/ftr_line.png";
    $pdf->Image($ftr_line, $pdf->GetX() + 50, $pdf->GetY() + 102, 150, 0);
    $pdf->SetFont('Times', 'I', 15);
    $pdf->SetLeftMargin(40);
    $pdf->GetY() + 60;
    $pdf->Ln(62);
    $pdf->Cell(320, 8, $UniqueString, '0', '1', 'L');
    //$pdf->Ln(6);
    $pdf->SetFont('Arial', 'I', 8);
    $pdf->SetTextColor(0, 0, 0);
    $filename = str_replace(' ', '-', $data3->contributor_name) . '-eSim-Circuit-Simulation-Certificate.pdf';
    $file = $path . '/pdf/temp_certificate/' . $proposal_id . '_' . $filename;
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
public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
}
}
?>
