<?php /**
 * @file
 * Contains \Drupal\circuit_simulation\Controller\DefaultController.
 */

namespace Drupal\circuit_simulation\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Link;
use Drupal\Core\Url;
use Symfony\Component\HttpFoundation\Response;
use Drupal\Core\Routing\RouteMatchInterface;
use Drupal\Core\Database\Database;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Drupal\Core\File\FileSystemInterface;
use Drupal\Service;
use Drupal\user\Entity\User;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Form\FormBuilderInterface;
use Drupal\Core\Render\Markup;

/**
 * Default controller for the circuit_simulation module.
 */
class DefaultController extends ControllerBase {

  public function circuit_simulation_proposal_pending() {
    /* get pending proposals to be approved */
    $pending_rows = [];
    $query = \Drupal::database()->select('esim_circuit_simulation_proposal');
    $query->fields('esim_circuit_simulation_proposal');
    $query->condition('approval_status', 0);
    $query->orderBy('id', 'DESC');
    $pending_q = $query->execute();
    while ($pending_data = $pending_q->fetchObject()) {
      $approval_url = Link::fromTextAndUrl(
  $this->t('Approve'),
  Url::fromUri('internal:/circuit-simulation-project/manage-proposal/approve/' . $pending_data->id)
)->toString();
      $edit_url =  Link::fromTextAndUrl(
  $this->t('Edit'),
  Url::fromUri('internal:/circuit-simulation-project/manage-proposal/edit/' . $pending_data->id)
)->toString();
      $mainLink = t('@linkApprove | @linkReject', array('@linkApprove' => $approval_url, '@linkReject' => $edit_url));
      $pending_rows[$pending_data->id] = [
        date('d-m-Y', $pending_data->creation_date),
        Link::fromTextAndUrl($pending_data->contributor_name, Url::fromRoute('entity.user.canonical', ['user' => $pending_data->uid])),
        $pending_data->project_title,
        $mainLink
      ];

    } //$pending_data = $pending_q->fetchObject()
  /* check if there are any pending proposals */
    /*if (!$pending_rows) {
      $msg = \Drupal::messenger()->addStatus(t('There are no pending proposals.'));
      return $msg;
    } //!$pending_rows*/
    $pending_header = [
      'Date of Submission',
      'Student Name',
      'Title of the Project',
      'Action',
    ];
    $output =  [
      '#type' => 'table',
      '#header' => $pending_header,
      '#rows' => $pending_rows,
      '#empty' => 'no rows found',
    ];
    return $output;
  }

  public function circuit_simulation_proposal_all() {
    /* get pending proposals to be approved */
    $proposal_rows = [];
    $query = \Drupal::database()->select('esim_circuit_simulation_proposal');
    $query->fields('esim_circuit_simulation_proposal');
    $query->orderBy('id', 'DESC');
    $proposal_q = $query->execute();
    while ($proposal_data = $proposal_q->fetchObject()) {
      $approval_status = '';
      switch ($proposal_data->approval_status) {
        case 0:
          $approval_status = 'Pending';
          break;
        case 1:
          $approval_status = 'Approved';
          break;
        case 2:
          $approval_status = 'Dis-approved';
          break;
        case 3:
          $approval_status = 'Completed';
          break;
        default:
          $approval_status = 'Unknown';
          break;
      } //$proposal_data->approval_status
      if ($proposal_data->actual_completion_date == 0) {
        $actual_completion_date = "Not Completed";
      } //$proposal_data->actual_completion_date == 0
      else {
        $actual_completion_date = date('d-m-Y', $proposal_data->actual_completion_date);
      }
       $approval_url = Link::fromTextAndUrl(
  $this->t('Status'),
  Url::fromUri('internal:/circuit-simulation-project/manage-proposal/status/' . $proposal_data->id)
)->toString();
      //$approval_url = Link::fromTextAndUrl('Status', Url::fromRoute('om_flowsheet.proposal_status_form',['id'=>$proposal_data->id]))->toString();
      $edit_url =  Link::fromTextAndUrl('Edit', Url::fromUri('internal:/circuit-simulation-project/manage-proposal/edit/' . $proposal_data->id))->toString();
      $mainLink = t('@linkApprove | @linkReject', array('@linkApprove' => $approval_url, '@linkReject' => $edit_url));
      $proposal_rows[$proposal_data->id] = [
        $actual_completion_date,
        Link::fromTextAndUrl($proposal_data->contributor_name, Url::fromRoute('entity.user.canonical', ['user' => $proposal_data->uid])),
        $proposal_data->project_title,
        $actual_completion_date,
        $approval_status,
        $mainLink
      ];
    } //$proposal_data = $proposal_q->fetchObject()
	/* check if there are any pending proposals */
    // if (!$proposal_rows) {
    //   \Drupal::messenger()->addStatus(t('There are no proposals.'));
    //   return '';
    // } //!$proposal_rows
    $proposal_header = [
      'Date of Submission',
      'Student Name',
      'Title of the circuit-simulation project',
      'Date of Completion',
      'Status',
      'Action',
    ];
    $output =  [
      '#type' => 'table',
      '#header' => $proposal_header,
      '#rows' => $proposal_rows,
      '#empty' => 'no rows found',
    ];

    return $output;
  }

  public function circuit_simulation_approved_tab() {
    $markup = "";
    $result = \Drupal::database()->query("SELECT * from esim_circuit_simulation_proposal where id not in (select proposal_id from esim_circuit_simulation_submitted_abstracts) AND approval_status = 1 order by approval_date desc");
    $rows = $result->fetchAll();
$i = count($rows);
    if ($i == 0) {
      $markup .= "Work is in progress for the following circuit simulation under Circuit Simulation Project: " . $i . "<hr>";
    } //$result->rowCount() == 0
    else {
      $markup .= "Work is in progress for the following circuit simulation under Circuit Simulation Project: " . $i . "<hr>";
      $preference_rows = [];
      //$i = 1;
      foreach($rows as $row) {
        $approval_date = date("d-M-Y", $row->approval_date);
        $preference_rows[] = [
          $i,
          $row->project_title,
          $row->contributor_name,
          $row->university,
          $approval_date,
        ];
        $i--;
      } //$row = $result->fetchObject()
      $preference_header = [
        'No',
        'Circuit Simulation Project',
        'Contributor Name',
        'Institute',
        'Date of Approval',
      ];
      $page_content = [
        '#type' => 'table',
        '#rows' => $preference_rows,
        '#header' => $preference_header
      ];

    }
    return [
      'markup' => [
        '#markup' => $markup,
      ],
      'table' => $page_content,
    ];
      
  }

  public function circuit_simulation_uploaded_tab() {
    $markup = "";
    $result = \Drupal::database()->query("SELECT dfp.project_title, dfp.contributor_name, dfp.id, dfp.university, dfa.abstract_upload_date, dfa.abstract_approval_status from esim_circuit_simulation_proposal as dfp JOIN esim_circuit_simulation_submitted_abstracts as dfa on dfa.proposal_id = dfp.id where dfp.id in (select proposal_id from esim_circuit_simulation_submitted_abstracts) AND approval_status = 1 order by dfa.abstract_upload_date DESC");
    $rows = $result->fetchAll();
$i = count($rows);
    if ($i == 0) {
      $markup .= "Uploaded Proposals under Circuit Simulation Project<hr>";
    }
    else {
      $markup .= "Uploaded Proposals under Circuit Simulation Project: " . $i . "<hr>";
      $preference_rows = [];
      //$i = 1;
      foreach($rows as $row) {
        $abstract_upload_date = date("d-M-Y", $row->abstract_upload_date);
        $preference_rows[] = [
          $i,
          $row->project_title,
          $row->contributor_name,
          $row->university,
          $abstract_upload_date,
        ];
        $i--;
      }
      $preference_header = [
        'No',
        'Circuit Simulation Project',
        'Contributor Name',
        'University / Institute',
        'Date of file submission',
      ];
      $page_content = [
        '#type' => 'table',
        '#rows' => $preference_rows,
        '#header' => $preference_header
      ];

    }
    return [
      'markup' => [
        '#markup' => $markup,
      ],
      'table' => $page_content,
    ];
  }

  public function circuit_simulation_abstract() {
    $user = \Drupal::currentUser();
    $return_html = "";
    $proposal_data = \Drupal::service('circuit_simulation_global')->circuit_simulation_get_proposal();
    if (!$proposal_data) {
      //drupal_goto('');
      return;
    } //!$proposal_data
    //$return_html .= l('Upload abstract', 'circuit-simulation-project/abstract-code/upload') . '<br />';
	/* get experiment list */
    $query = \Drupal::database()->select('esim_circuit_simulation_submitted_abstracts');
    $query->fields('esim_circuit_simulation_submitted_abstracts');
    $query->condition('proposal_id', $proposal_data->id);
    $abstracts_q = $query->execute()->fetchObject();
    $query_pro = \Drupal::database()->select('esim_circuit_simulation_proposal');
    $query_pro->fields('esim_circuit_simulation_proposal');
    $query_pro->condition('id', $proposal_data->id);
    $abstracts_pro = $query_pro->execute()->fetchObject();
    $query_pdf = \Drupal::database()->select('esim_circuit_simulation_submitted_abstracts_file');
    $query_pdf->fields('esim_circuit_simulation_submitted_abstracts_file');
    $query_pdf->condition('proposal_id', $proposal_data->id);
    $query_pdf->condition('filetype', 'A');
    $abstracts_pdf = $query_pdf->execute()->fetchObject();
    //var_dump($abstracts_pdf);die;
    if ($abstracts_pdf) {
      if ($abstracts_pdf->filename != "NULL" || $abstracts_pdf->filename != "") {
        $abstract_filename = $abstracts_pdf->filename;
        //$abstract_filename = l($abstracts_pdf->filename, 'circuit-simulation-project/download/project-file/' . $proposal_data->id);
      } //$abstracts_pdf->filename != "NULL" || $abstracts_pdf->filename != ""
      else {
        $abstract_filename = "File not uploaded";
      }
    } //$abstracts_pdf == TRUE
    else {
      $abstract_filename = "File not uploaded";
    }
    $query_process = \Drupal::database()->select('esim_circuit_simulation_submitted_abstracts_file');
    $query_process->fields('esim_circuit_simulation_submitted_abstracts_file');
    $query_process->condition('proposal_id', $proposal_data->id);
    $query_process->condition('filetype', 'S');
    $abstracts_query_process = $query_process->execute()->fetchObject();
    if ($abstracts_query_process) {
      if ($abstracts_query_process->filename != "NULL" || $abstracts_query_process->filename != "") {
        $abstracts_query_process_filename = $abstracts_query_process->filename;
        //$abstracts_query_process_filename = l($abstracts_query_process->filename, 'circuit-simulation-project/download/project-file/' . $proposal_data->id); 
      } //$abstracts_query_process->filename != "NULL" || $abstracts_query_process->filename != ""
      else {
        $abstracts_query_process_filename = "File not uploaded";
      }
      if ($abstracts_q->is_submitted == '') {
        // @FIXME
// l() expects a Url object, created from a route name or external URI.
 $url = Link::fromTextAndUrl(
  'Upload Abstract',
  Url::fromUri('internal:/circuit-simulation-project/abstract-code/upload')
)->toString();

      } //$abstracts_q->is_submitted == ''
      else {
        if ($abstracts_q->is_submitted == 1) {
          $url = "";
        } //$abstracts_q->is_submitted == 1
        else {
          if ($abstracts_q->is_submitted == 0) {
            // @FIXME
// l() expects a Url object, created from a route name or external URI.
// $url = l('Edit', 'circuit-simulation-project/abstract-code/upload');
$url = Link::fromTextAndUrl(
  'Edit',
  Url::fromUri('internal:/circuit-simulation-project/abstract-code/upload')
)->toString();
          }
        }
      } //$abstracts_q->is_submitted == 0
    } //$abstracts_query_process == TRUE
    else {
      // @FIXME
// l() expects a Url object, created from a route name or external URI.
// $url = l('Upload abstract', 'circuit-simulation-project/abstract-code/upload');
$url = Link::fromTextAndUrl(
  'Upload Abstract',
  Url::fromUri('internal:/circuit-simulation-project/abstract-code/upload')
)->toString();
      $abstracts_query_process_filename = "File not uploaded";
    }
    $return_html .= '<strong>Contributor Name:</strong><br />' . $proposal_data->name_title . ' ' . $proposal_data->contributor_name . '<br /><br />';
    $return_html .= '<strong>Title of the Circuit Simulation Project:</strong><br />' . $proposal_data->project_title . '<br /><br />';
    $return_html .= '<strong>Uploaded abstract of the project:</strong><br />' . $abstract_filename . '<br /><br />';
    $return_html .= '<strong>Uploaded project files:</strong><br />' . $abstracts_query_process_filename . '<br /><br />';
    $return_html .= $url . '<br />';
    return [
      '#type' => 'markup',
      '#markup' => $return_html,
    ];
  }

  public function circuit_simulation_download_full_project() {
    $service = \Drupal::service('circuit_simulation_global');
    $user = \Drupal::currentUser();
    //$id = arg(3);
    $id = \Drupal::routeMatch()->getParameter('proposal_id');
    $root_path = $service->circuit_simulation_path();
    //var_dump($root_path);die;
    $query = \Drupal::database()->select('esim_circuit_simulation_proposal');
    $query->fields('esim_circuit_simulation_proposal');
    $query->condition('id', $id);
    $circuit_simulation_q = $query->execute();
    $circuit_simulation_data = $circuit_simulation_q->fetchObject();
    $CIRCUITSIMULATION_PATH = $circuit_simulation_data->directory_name . '/';
    /* zip filename */
    $zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
    /* creating zip archive on the server */
    $zip = new \ZipArchive();
    $zip->open($zip_filename, \ZipArchive::CREATE);
    $query = \Drupal::database()->select('esim_circuit_simulation_submitted_abstracts_file');
    $query->fields('esim_circuit_simulation_submitted_abstracts_file');
    $query->condition('proposal_id', $id);
    $project_files = $query->execute();
    while ($esim_project_files = $project_files->fetchObject()) {
      $zip->addFile($root_path . $CIRCUITSIMULATION_PATH . 'project_files/' . $esim_project_files->filepath, $CIRCUITSIMULATION_PATH . str_replace(' ', '_', basename($esim_project_files->filename)));
    }
    $zip_file_count = $zip->numFiles;
    $zip->close();
    if ($zip_file_count > 0) {
      if ($user->uid) {
        /* download zip file */
        header('Content-Type: application/zip');
        header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $circuit_simulation_data->project_title) . '.zip"');
        header('Content-Length: ' . filesize($zip_filename));
        ob_clean();
        ob_end_flush();
        readfile($zip_filename);
        unlink($zip_filename);
        /*flush();
			ob_end_flush();
			ob_clean();*/

      } //$user->uid
      else {
        header('Content-Type: application/zip');
        header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $circuit_simulation_data->project_title) . '.zip"');
        header('Content-Length: ' . filesize($zip_filename));
        header("Content-Transfer-Encoding: binary");
        header('Expires: 0');
        header('Pragma: no-cache');
        //ob_end_flush();
        ob_clean();
        //flush();
        ob_end_flush();
        readfile($zip_filename);
        unlink($zip_filename);
      }
    } //$zip_file_count > 0
    else {
      \Drupal::messenger()->addError("There are no circuit simulation project in this proposal to download");
      return new RedirectResponse('/circuit-simulation-project/full-download/project/' . $proposal_id);
      //drupal_goto('circuit-simulation-project/full-download/project');
    }
  }

  public function circuit_simulation_download_proposals() {
    $service = \Drupal::service('circuit_simulation_global');
    $root_path = $service->circuit_simulation_path();

    $result = \Drupal::database()->query("SELECT e.contributor_name as contirbutor_name, u.mail as email_id, e.project_title as title, e.contact_no as contact, e.university as university, from_unixtime(creation_date,'%d-%m-%Y') as creation, from_unixtime(approval_date,'%d-%m-%Y') as approval, from_unixtime(actual_completion_date,'%d-%m-%Y') as year, e.approval_status as status FROM esim_circuit_simulation_proposal as e JOIN users as u ON e.uid = u.uid ORDER BY actual_completion_date DESC");

    //var_dump($result->rowCount());die();
    //$all_proposals_q = $result->execute();
    $participants_proposal_id_file = $root_path . "participants-proposals.csv";
    $fp = fopen($participants_proposal_id_file, "w");
    /* making the first row */
    $items = [
      'Contirbutor Name',
      'Email ID',
      'Circuit Simulation Title',
      'University',
      'Contact',
      'Date of Creation',
      'Date of Approval',
      'Date of Completion',
      'Status of the proposal',
    ];
    fputcsv($fp, $items);
    while ($row = $result->fetchObject()) {
      $status = '';
      switch ($row->status) {
        case 0:
          $status = 'Pending';
          break;
        case 1:
          $status = 'Approved';
          break;
        case 2:
          $status = 'Dis-approved';
          break;
        case 3:
          $status = 'Completed';
          break;
        default:
          $status = 'Unknown';
          break;
      } //$row->status
      if ($row->year == 0) {
        $year = "Not Completed";
      } //$row->year == 0
      else {
        $year = date('d-m-Y', $row->year);
      }

      $items = [
        $row->contirbutor_name,
        $row->email_id,
        $row->title,
        $row->university,
        $row->contact,
        $row->creation,
        $row->approval,
        $row->year,
        $status,
      ];
      fputcsv($fp, $items);
    }
    fclose($fp);
    if ($participants_proposal_id_file) {
      ob_clean();
      header("Pragma: public");
      header("Expires: 0");
      header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
      header("Cache-Control: public");
      header("Content-Description: File Transfer");
      header('Content-Type: application/csv');
      header('Content-disposition: attachment; filename=participants-proposals.csv');
      header('Content-Length:' . filesize($participants_proposal_id_file));
      header("Content-Transfer-Encoding: binary");
      header('Expires: 0');
      header('Pragma: no-cache');
      readfile($participants_proposal_id_file);
      /*ob_end_flush();
            ob_clean();
            flush();*/
    }
  }

  public function esim_circuit_simulation_download_completed_proposals() {
    $output = "";
    // @FIXME
    // l() expects a Url object, created from a route name or external URI.
    // $output .= "Click ".l("here","/circuit-simulation-project/download-proposals"). " to download the Proposals of the participants" ."<h4>";


    return $output;

  }

  public function circuit_simulation_completed_proposals_all() {
    $output = "";
    $count_query = \Drupal::database()->select('esim_circuit_simulation_proposal', 't')
  ->condition('approval_status', 3)
  ->countQuery();
  $i = $count_query->execute()->fetchField(); 
    $query = \Drupal::database()->select('esim_circuit_simulation_proposal');
    $query->fields('esim_circuit_simulation_proposal');
    $query->condition('approval_status', 3);
    $query->orderBy('actual_completion_date', 'DESC');
    //$query->condition('is_completed', 1);
    $result = $query->execute();

    //var_dump($esim_project_abstract);die;
    // if ($result->rowCount() == 0) {
    //   $output .= "Work has been completed for the following circuit simulation. We welcome your contributions." . "<hr>";

    // } //$result->rowCount() == 0
    // else {
      $output .= "Work has been completed for the following circuit simulation. We welcome your contributions." . "<hr>";
      $preference_rows = [];
      while ($row = $result->fetchObject()) {
        //var_dump($row);die;
        $completion_date = date("Y", $row->actual_completion_date);
        $url = Url::fromUri('internal:/circuit-simulation-project/esim-circuit-simulation-run/' . $row->id);
        $link = Link::fromTextAndUrl($row->project_title, $url)->toString();

        $preference_rows[] = array(
                $i,
                $link,
                $row->contributor_name,
                $row->university,
                $completion_date
              );

        $i--;
      } //$row = $result->fetchObject()
      $preference_header = [
        'No',
        'Flowsheet Project',
        'Contributor Name',
        'University / Institute',
        'Year of Completion',
      ];
      $output =  [
      '#type' => 'table',
      '#header' => $preference_header,
      '#rows' => $preference_rows,
      '#empty' => 'We welcome your contributions to the eSim Circuit Simulation Project',
    ];

   //}
    return $output;
  }

  public function circuit_simulation_completed_pspice_to_kicad() {
    $output = "";
    $count_query = \Drupal::database()->select('pspice_to_kicad_circuits', 't')
  ->countQuery();
  $i = $count_query->execute()->fetchField(); 
    $query = \Drupal::database()->select('pspice_to_kicad_circuits');
    $query->fields('pspice_to_kicad_circuits');
    //$query->condition('is_completed', 1);
    $result = $query->execute();

    //var_dump($esim_project_abstract);die;
    if ($i == 0) {
      $markup = "<h4 dir='ltr'><span style='color:#008000'><strong>PSpice to KiCad Converter</strong></span></h4>

<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>This feature converts a schematic file created using PSpice&reg; to KiCad format. The converted schematic file is compatible with KiCad for PCB layout. You can also </span><span style='background-color:#fcfcfc; color:#000000; font-family:times new roman; font-size:12pt'>create a netlist and simulate using Ngspice. The source code for this converter is available <span style='text-decoration: underline;'><a href='https://github.com/FOSSEE/eSim_PSpice_to_KiCad_Python_Parser' target='_blank'>here</a>.</span></p>

<p dir='ltr'><span style='color:#008000'><strong>How to convert?</strong></span></p>

<ol>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>Login to eSim website</span><a href='https://esim.fossee.in/' style='text-decoration:none;' target='_blank'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'> </span><u>here</u></a><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>Upload the PSpice Schematic files by clicking <a href='https://esim.fossee.in/pspice-to-kicad/add' style='text-decoration:none;' target='_blank'><u>here</u></a><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>. </span><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>&nbsp;&nbsp;&nbsp; </span></span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>The PSpice schematic should be in standard format. Refer <a href='https://static.fossee.in/esim/manuals/trafo3ph_Dyn11.sch' target='_blank'>here</a>, for a few PSpice schematic samples.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>Your files will be converted to the KiCad schematic format. Once the conversion is complete, you will be intimated through your registered email id.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>You can use the KiCad schematic further for Simulation and for PCB generation. Follow the instructions <a href='https://static.fossee.in/esim/manuals/Instructions_PSpice_to_KiCad.pdf' target='_blank'>here</a>.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'> Converted files after verification are made available for download in the table below.</p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>  We welcome your contributions to this page</p>
	</li>
</ol>" . "<hr>";

    } //$result->rowCount() == 0
    else {
      $markup = "<h4 dir='ltr'><span style='color:#008000'><strong>PSpice to KiCad Converter</strong></span></h4>

<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>This feature converts a schematic file created using PSpice&reg; to KiCad format. The converted schematic file is compatible with KiCad for PCB layout. You can also </span><span style='background-color:#fcfcfc; color:#000000; font-family:times new roman; font-size:12pt'>create a netlist and simulate using Ngspice. The source code for this converter is available <span style='text-decoration: underline;'><a href='https://github.com/FOSSEE/eSim_PSpice_to_KiCad_Python_Parser' target='_blank'>here</a>.</span></p>

<p dir='ltr'><span style='color:#008000'><strong>How to convert?</strong></span></p>

<ol>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>Login to eSim website</span><a href='https://esim.fossee.in/' style='text-decoration:none;' target='_blank'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'> </span><u>here</u></a><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>Upload the PSpice Schematic files by clicking <a href='https://esim.fossee.in/pspice-to-kicad/add' style='text-decoration:none;' target='_blank'><u>here</u></a><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>. </span><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>&nbsp;&nbsp;&nbsp; </span></span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>The PSpice schematic should be in standard format. Refer <a href='https://static.fossee.in/esim/manuals/trafo3ph_Dyn11.sch' target='_blank'>here</a>, for a few PSpice schematic samples.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>Your files will be converted to the KiCad schematic format. Once the conversion is complete, you will be intimated through your registered email id.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>You can use the KiCad schematic further for Simulation and for PCB generation. Follow the instructions <a href='https://static.fossee.in/esim/manuals/Instructions_PSpice_to_KiCad.pdf' target='_blank'>here</a>.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'> Converted files after verification are made available for download in the table below.</p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>  We welcome your contributions to this page</p>
	</li>
</ol>" . "<hr>";
      $preference_rows = [];
     // $i = $result->rowCount();
      while ($row = $result->fetchObject()) {
        
$preference_rows[] = array(
				$i,
        $url = Link::fromTextAndUrl($row->name_of_circuit, Url::fromUri('https://static.fossee.in/esim/converters/pspicetokicad_PAGE_all/' . $row->filename . '.tar.gz'))->toString()
				//l($row->name_of_circuit, 'https://static.fossee.in/esim/converters/pspicetokicad_PAGE_all/' . $row->filename . '.tar.gz')
			);

        $i--;
      } //$row = $result->fetchObject()
      $preference_header = [
        'No',
        'Name of the Circuit',
      ];
      $output = [
        '#type' => 'table',
        '#header'=> $preference_header,
        '#rows' => $preference_rows
      ];
    }
    return [
      'markup' => [
        '#markup' => $markup,
      ],
      'table' => $output,
    ];
    //return $output;
  }

  public function circuit_simulation_completed_ltspice_to_kicad() {
    $output = "";
    $count_query = \Drupal::database()->select('ltspice_to_kicad_circuits', 't')
  ->countQuery();
  $i = $count_query->execute()->fetchField();
    $query = \Drupal::database()->select('ltspice_to_kicad_circuits');
    $query->fields('ltspice_to_kicad_circuits');
    $result = $query->execute();
    $preference_rows = [];
    $markup = "
<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>The below files are converted from LTSpice to KiCad. The converted schematic file is compatible with KiCad for PCB layout. You can also create a netlist and simulate using Ngspice. 
<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>This is a Beta Release. Please mail us at contact-esim@fossee.in in case of any issues.</span</p>
<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>The link to the LTSpice to KiCad Converter will be released soon.</span></p>
<hr>";
    //$i = $result->rowCount();
    while ($row = $result->fetchObject()) {
$preference_rows[] = [
				$i,
				Link::fromTextAndUrl(ltrim($row->circuit_full_name), Url::fromUri('https://static.fossee.in/esim/converters/ltspice_to_kicad/' . ltrim($row->filename)))->toString()
				//$row->university,
				//$completion_date
			];

      $i--;
    } //$row = $result->fetchObject()
    $preference_header = [
      'No',
      'Name of the Circuit',
    ];
    $output = [
        '#type' => 'table',
        '#header'=> $preference_header,
        '#rows' => $preference_rows
      ];
    
    return [
      'markup' => [
        '#markup' => $markup,
      ],
      'table' => $output,
    ];
  }

  public function circuit_simulation_progress_all() {
    $page_content = "";
$count_query = \Drupal::database()->select('esim_circuit_simulation_proposal', 't')
  ->condition('approval_status', '1')
  ->condition('is_completed', 0)
  ->countQuery();
  $i = $count_query->execute()->fetchField(); 
    $query = \Drupal::database()->select('esim_circuit_simulation_proposal');
    $query->fields('esim_circuit_simulation_proposal');
    $query->condition('approval_status', 1);
    $query->condition('is_completed', 0);
    $query->orderBy('approval_date', 'DESC');
    $result = $query->execute();
    if ($i == 0) {
      $markup = "Work is in progress for the following circuit simulation under Circuit Simulation Project<hr>";
    } //$result->rowCount() == 0
    else {
      $markup = "Work is in progress for the following circuit simulation under Circuit Simulation Project<hr>";
      $preference_rows = [];
      while ($row = $result->fetchObject()) {
        $approval_date = date("Y", $row->approval_date);
        $preference_rows[] = [
          $i,
          $row->project_title,
          $row->contributor_name,
          $row->university,
          $approval_date,
        ];
        $i--;
      } //$row = $result->fetchObject()
      $preference_header = [
        'No',
        'Circuit Simulation Project',
        'Contributor Name',
        'Institute',
        'Year',
      ];
      $page_content = [
        '#type' => 'table',
        '#header' => $preference_header,
        '#rows' => $preference_rows
      ];

    }
    return [
      'markup' => [
        '#markup' => $markup,
      ],
      'table' => $page_content,
    ];
  }

  public function circuit_simulation_download_upload_file() {
    $service = \Drupal::service('circuit_simulation_global');
    $user = \Drupal::currentUser();
    //$id = arg(3);
    $id = \Drupal::routeMatch()->getParameter('proposal_id');
    $root_path = $service->circuit_simulation_path();
    $query = \Drupal::database()->select('esim_circuit_simulation_proposal');
    $query->fields('esim_circuit_simulation_proposal');
    $query->condition('id', $proposal_id);
    $query->range(0, 1);
    $result = $query->execute();
    $circuit_simulation_upload_file = $result->fetchObject();
    $samplecodename = substr($circuit_simulation_upload_file->samplefilepath, strrpos($circuit_simulation_upload_file->samplefilepath, '/') + 1);
    ob_clean();
    header("Pragma: public");
    header("Expires: 0");
    header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
    header("Cache-Control: public");
    header("Content-Description: File Transfer");
    header('Content-Type: application/pdf');
    header('Content-disposition: attachment; filename="' . $samplecodename . '"');
    header('Content-Length: ' . filesize($root_path . $circuit_simulation_upload_file->samplefilepath));
    header("Content-Transfer-Encoding: binary");
    header('Expires: 0');
    header('Pragma: no-cache');
    ob_clean();
    readfile($root_path . $circuit_simulation_upload_file->samplefilepath);
    //ob_end_flush();

    //flush();
  }

  public function esim_circuit_simulation_project_files() {

    $service = \Drupal::service('circuit_simulation_global');
    $user = \Drupal::currentUser();
    //$id = arg(3);
    $proposal_id = \Drupal::routeMatch()->getParameter('proposal_id');
    $root_path = $service->circuit_simulation_path();
    $query = \Drupal::database()->select('esim_circuit_simulation_submitted_abstracts_file');
    $query->fields('esim_circuit_simulation_submitted_abstracts_file');
    $query->condition('proposal_id', $proposal_id);
    $result = $query->execute();
    $esim_circuit_simulation_project_files = $result->fetchObject();
    //var_dump($esim_circuit_simulation_project_files);die;
    $query1 = \Drupal::database()->select('esim_circuit_simulation_proposal');
    $query1->fields('esim_circuit_simulation_proposal');
    $query1->condition('id', $proposal_id);
    $result1 = $query1->execute();
    $circuit_simulation = $result1->fetchObject();
    $directory_name = $circuit_simulation->directory_name . '/project_files/';
    $samplecodename = $esim_circuit_simulation_project_files->filename;
    ob_clean();
    header("Pragma: public");
    header("Expires: 0");
    header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
    header("Cache-Control: public");
    header("Content-Description: File Transfer");
    header("Content-Type: application/pdf");
    header('Content-disposition: attachment; filename="' . $samplecodename . '"');
    header("Content-Length: " . filesize($root_path . $directory_name . $samplecodename));
    header("Content-Transfer-Encoding: binary");
    header("Expires: 0");
    header("Pragma: no-cache");
    ob_clean();
    readfile($root_path . $directory_name . $samplecodename);
    //ob_end_flush();
    //ob_clean();
  }

  public function _list_circuit_simulation_certificates() {
    $user = \Drupal::currentUser();
    $query_id = \Drupal::database()->query("SELECT id FROM esim_circuit_simulation_proposal WHERE approval_status=3 AND uid= :uid", [
      ':uid' => $user->uid
      ]);
    $exist_id = $query_id->fetchObject();
    //var_dump($exist_id->id);die;
    if ($exist_id) {
      if ($exist_id->id) {
        if ($exist_id->id < 2) {
          \Drupal::messenger()->addStatus('<strong>You need to propose a <a href="https://esim.fossee.in/circuit-simulation-project/proposal">Circuit Simulation Proposal</a></strong> or if you have already proposed then your Circuit Simulation is under reviewing process');
          return '';
        } //$exist_id->id < 3
        else {
          $search_rows = [];
          global $output;
          $output = '';
          $query3 = \Drupal::database()->query("SELECT id,project_title,contributor_name FROM esim_circuit_simulation_proposal WHERE approval_status=3 AND uid= :uid", [
            ':uid' => $user->uid
            ]);
          while ($search_data3 = $query3->fetchObject()) {
            if ($search_data3->id) {
              // @FIXME
// l() expects a Url object, created from a route name or external URI.
// $search_rows[] = array(
// 						$search_data3->project_title,
// 						$search_data3->contributor_name,
// 						l('Download Certificate', 'circuit-simulation-project/certificates/generate-pdf/' . $search_data3->id)
// 					);

            } //$search_data3->id
          } //$search_data3 = $query3->fetchObject()
          if ($search_rows) {
            $search_header = [
              'Project Title',
              'Contributor Name',
              'Download Certificates',
            ];
            // @FIXME
            // theme() has been renamed to _theme() and should NEVER be called directly.
            // Calling _theme() directly can alter the expected output and potentially
            // introduce security issues (see https://www.drupal.org/node/2195739). You
            // should use renderable arrays instead.
            // 
            // 
            // @see https://www.drupal.org/node/2195739
            // $output        = theme('table', array(
            // 					'header' => $search_header,
            // 					'rows' => $search_rows
            // 				));

            return $output;
          } //$search_rows
          else {
            echo ("Error");
            return '';
          }
        }
      }
    } //$exist_id->id
    else {
      \Drupal::messenger()->addStatus('<strong>You need to propose a <a href="https://esim.fossee.in/circuit-simulation-project/proposal">Circuit Simulation Proposal</a></strong> or if you have already proposed then your Circuit Simulation is under reviewing process');
      $page_content = "<span style='color:red;'> No certificate available </span>";
      return $page_content;
    }
  }

  public function _list_circuit_simulation_custom_certificates() {
    $user = \Drupal::currentUser();
    $query_id = \Drupal::database()->query("SELECT id FROM esim_circuit_simulation_proposal WHERE approval_status=3");
    $exist_id = $query_id->fetchObject();
    if ($exist_id) {
      if ($exist_id->id) {
        if ($exist_id->id < 3) {
          \Drupal::messenger()->addStatus('<strong>There are no proposals with mentors</strong>');
          return '';
        } //$exist_id->id < 3
        else {
          $search_rows = [];
          global $output;
          $output = '';
          $query3 = \Drupal::database()->query("SELECT id,project_guide_name,project_title FROM esim_circuit_simulation_proposal WHERE project_guide_name != '' AND approval_status=3");
          $i = 1;
          while ($search_data3 = $query3->fetchObject()) {
            // @FIXME
// l() expects a Url object, created from a route name or external URI.
// $search_rows[] = array(
// 						$i,
// 						$search_data3->project_title,
// 						$search_data3->project_guide_name,
// 						l('Download Certificate', 'circuit-simulation-project/certificates-custom/pdf/' . $search_data3->id)
// 					);

            $i++;
            //$search_data3->id
          } //$search_data3 = $query3->fetchObject()
          if ($search_rows) {
            $search_header = [
              'No',
              'Project Title',
              'Project Guide Name',
              'Download Certificates',
            ];
            // @FIXME
            // theme() has been renamed to _theme() and should NEVER be called directly.
            // Calling _theme() directly can alter the expected output and potentially
            // introduce security issues (see https://www.drupal.org/node/2195739). You
            // should use renderable arrays instead.
            // 
            // 
            // @see https://www.drupal.org/node/2195739
            // $output        = theme('table', array(
            // 					'header' => $search_header,
            // 					'rows' => $search_rows
            // 				));

            return $output;
          } //$search_rows
          else {
            echo ("Error");
            return '';
          }
        }
      }
    } //$exist_id->id
    else {
      \Drupal::messenger()->addStatus('<strong>There are no proposals with mentors</strong>');
      $page_content = "<span style='color:red;'> No certificate available </span>";
      return $page_content;
    }
  }

  public function verify_certificates($qr_code = 0) {
    $qr_code = arg(3);
    $page_content = "";
    if ($qr_code) {
      $page_content = verify_qrcode_fromdb($qr_code);
    } //$qr_code
    else {
      $verify_certificates_form = \Drupal::formBuilder()->getForm("verify_certificates_form");
      $page_content = \Drupal::service("renderer")->render($verify_certificates_form);
    }
    return $page_content;
  }

}
