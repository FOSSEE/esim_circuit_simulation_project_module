<?php

/**
 * @file
 * Contains \Drupal\circuit_simulation\Form\CircuitSimulationUploadAbstractCodeForm.
 */

namespace Drupal\circuit_simulation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;
use Drupal\Core\Link;
use Drupal\Core\Url;
use Drupal\Core\Routing\TrustedRedirectResponse;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Drupal\Core\Routing\RouteMatchInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\Core\Database\Database;
use Drupal\Core\Messenger\MessengerInterface;
use Drupal\Core\Mail\MailManager;

class CircuitSimulationUploadAbstractCodeForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'circuit_simulation_upload_abstract_code_form';
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $user = \Drupal::currentUser();
    $service = \Drupal::service("circuit_simulation_global");
    $form['#attributes'] = ['enctype' => "multipart/form-data"];
    /* get current proposal */
    //$proposal_id = (int) arg(3);
    $uid = $user->id();
    $query = \Drupal::database()->select('esim_circuit_simulation_proposal');
    $query->fields('esim_circuit_simulation_proposal');
    $query->condition('uid', $uid);
    $query->condition('approval_status', '1');
    $proposal_q = $query->execute();
    if ($proposal_q) {
      if ($proposal_data = $proposal_q->fetchObject()) {
        /* everything ok */
      } //$proposal_data = $proposal_q->fetchObject()
      else {
        $msg = \Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
         $response = new RedirectResponse(Url::fromUri('internal:/circuit-simulation-project/abstract-code')->toString());
    // Send the redirect response
    $response->send();
        return $msg;
      }
    } //$proposal_q
    else {
      $msg = \Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
      $response = new RedirectResponse(Url::fromUri('internal:/circuit-simulation-project/abstract-code')->toString());
    // Send the redirect response
    $response->send();
      return $msg;
    }
    $query = \Drupal::database()->select('esim_circuit_simulation_submitted_abstracts');
    $query->fields('esim_circuit_simulation_submitted_abstracts');
    $query->condition('proposal_id', $proposal_data->id);
    $abstracts_q = $query->execute()->fetchObject();
    if ($abstracts_q) {
      if ($abstracts_q->is_submitted == 1) {
       $msg = \Drupal::messenger()->addError(t('You have already submited your project files, hence you can not upload more code, for any query please write to us.'));
        $response = new RedirectResponse(Url::fromUri('internal:/circuit-simulation-project/abstract-code')->toString());
    // Send the redirect response
    $response->send();
    return $msg;
      } //$abstracts_q->is_submitted == 1
    } //$abstracts_q->is_submitted == 1
    $form['project_title'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->project_title,
      '#title' => t('Title of the Circuit Simulation Project'),
    ];
    $form['contributor_name'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->contributor_name,
      '#title' => t('Contributor Name'),
    ];
    $existing_uploaded_A_file = $service->default_value_for_uploaded_files('A', $proposal_data->id);
    if (!$existing_uploaded_A_file) {
      $existing_uploaded_A_file = new \stdClass();
      $existing_uploaded_A_file->filename = "No file uploaded";
    } //!$existing_uploaded_A_file
    $form['upload_an_abstract'] = [
      '#type' => 'file',
      '#title' => t('Upload an abstract of the project.'),
      '#description' => t('<span style="font-size:16px;">For a sample of the abstract <span style="font-size:20px;">&rarr;</span>
			<a href="http://static.fossee.in/esim/manuals/Analysis_of_frequency_response_of_the_BJT_amplifier_using_eSim.pdf"> 
			Click here</a></span>' . '<br />' . t('<span style="color:red;">Current File :</span> ' . $existing_uploaded_A_file->filename . '<br />' . t('<span style="color:red;">Allowed file extensions: ') . \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_abstract_upload_extensions') . '</span>')),
    ];
    $existing_uploaded_S_file = $service->default_value_for_uploaded_files("S", $proposal_data->id);
    if (!$existing_uploaded_S_file) {
      $existing_uploaded_S_file = new \stdClass();
      $existing_uploaded_S_file->filename = "No file uploaded";
    } //!$existing_uploaded_S_file
    $form['upload_circuit_simulation_developed_process'] = [
      '#type' => 'file',
      '#title' => t('Upload the Project Files'),
      '#description' => t('<span style="color:red;">Current File :</span> ' . $existing_uploaded_S_file->filename . '<br />Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions: ') . \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_project_files_extensions') . '</span>',
    ];
    $form['prop_id'] = [
      '#type' => 'hidden',
      '#value' => $proposal_data->id,
    ];/*
	$form['is_submitted'] = array(
		'#type' => 'checkboxes',
		//'#title' => t('Terms And Conditions'),
		'#options' => array(
			'status' => t('I have uploaded the project files')
		),
		'#required' => TRUE
	);*/
    $form['submit'] = [
      '#type' => 'submit',
      '#value' => t('Submit'),
    ];
    $form['cancel'] = array(
    		'#type' => 'item',
    		'#markup' => Link::fromTextAndUrl(
  'Cancel',
  Url::fromUri('internal:/circuit-simulation-project/abstract-code')
)->toString()
    	);

    return $form;
  }
  

  public function validateForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $service = \Drupal::service("circuit_simulation_global");
    if (isset($_FILES['files'])) {
      /* check if file is uploaded */
      $existing_uploaded_A_file = $service->default_value_for_uploaded_files('A', $form_state->getValue([
        'prop_id'
        ]));
      $existing_uploaded_S_file = $service->default_value_for_uploaded_files("S", $form_state->getValue([
        'prop_id'
        ]));
      if (!$existing_uploaded_S_file) {
        if (!($_FILES['files']['name']['upload_circuit_simulation_developed_process'])) {
          $form_state->setErrorByName('upload_circuit_simulation_developed_process', t('Please upload the file.'));
        }
      } //!$existing_uploaded_S_file
      if (!$existing_uploaded_A_file) {
        if (!($_FILES['files']['name']['upload_an_abstract'])) {
          $form_state->setErrorByName('upload_an_abstract', t('Please upload the file.'));
        }
      } //!$existing_uploaded_A_file
		/* check for valid filename extensions */
      if ($_FILES['files']['name']['upload_an_udc'] || $_FILES['files']['name']['upload_an_abstract'] || $_FILES['files']['name']['upload_circuit_simulation_developed_process']) {
        foreach ($_FILES['files']['name'] as $file_form_name => $file_name) {
          if ($file_name) {
            /* checking file type */
            if (strstr($file_form_name, 'upload_circuit_simulation_developed_process')) {
              $file_type = 'S';
            }
            else {
              if (strstr($file_form_name, 'upload_an_abstract')) {
                $file_type = 'A';
              }
            }
            $allowed_extensions_str = '';
            switch ($file_type) {
              case 'S':
                $allowed_extensions_str = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_project_files_extensions');
                break;
              case 'A':
                $allowed_extensions_str = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_abstract_upload_extensions');
                break;
            } //$file_type
            $allowed_extensions = explode(',', $allowed_extensions_str);
            $tmp_ext = explode('.', strtolower($_FILES['files']['name'][$file_form_name]));
            $temp_extension = end($tmp_ext);
            if (!in_array($temp_extension, $allowed_extensions)) {
              $form_state->setErrorByName($file_form_name, t('Only file with ' . $allowed_extensions_str . ' extensions can be uploaded.'));
            }
            if ($_FILES['files']['size'][$file_form_name] <= 0) {
              $form_state->setErrorByName($file_form_name, t('File size cannot be zero.'));
            }
            /* check if valid file name */
            if (!$service->circuit_simulation_check_valid_filename($_FILES['files']['name'][$file_form_name])) {
              $form_state->setErrorByName($file_form_name, t('Invalid file name specified. Only alphabets and numbers are allowed as a valid filename.'));
            }
          } //$file_name
        } //$_FILES['files']['name'] as $file_form_name => $file_name
      } //$_FILES['files']['name'] as $file_form_name => $file_name
    } //isset($_FILES['files'])
    // drupal_add_js('jQuery(document).ready(function () { alert("Hello!"); });', 'inline');
    // drupal_static_reset('drupal_add_js') ;
  }

  public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $service = \Drupal::service('circuit_simulation_global');
    $user = \Drupal::currentUser();
    //var_dump($user->id());die;
    $v = $form_state->getValues();
    $root_path = circuit_simulation_path();
    $proposal_data = $service->circuit_simulation_get_proposal();
    $proposal_id = $proposal_data->id;
    if (!$proposal_data) {
      $response = new RedirectResponse(Url::fromRoute('<front>')->toString());
      // Send the redirect response
      $response->send();
      return;
    } //!$proposal_data
    $proposal_id = $proposal_data->id;
    $proposal_directory = $proposal_data->directory_name;
    /* create proposal folder if not present */
    //$dest_path = $proposal_directory . '/';
    $dest_path_project_files = $proposal_directory . '/project_files/';
    if (!is_dir($root_path . $dest_path_project_files)) {
      mkdir($root_path . $dest_path_project_files);
    }
    $proposal_id = $proposal_data->id;
    $query_s = "SELECT * FROM esim_circuit_simulation_submitted_abstracts WHERE proposal_id = :proposal_id";
    $args_s = [":proposal_id" => $proposal_id];
    $query_s_result = \Drupal::database()->query($query_s, $args_s)->fetchObject();
    if (!$query_s_result) {
      /* creating solution database entry */
      $query = "INSERT INTO esim_circuit_simulation_submitted_abstracts (
	proposal_id,
	approver_uid,
	abstract_approval_status,
	abstract_upload_date,
	abstract_approval_date,
	is_submitted) VALUES (:proposal_id, :approver_uid, :abstract_approval_status,:abstract_upload_date, :abstract_approval_date, :is_submitted)";
      $args = [
        ":proposal_id" => $proposal_id,
        ":approver_uid" => 0,
        ":abstract_approval_status" => 0,
        ":abstract_upload_date" => time(),
        ":abstract_approval_date" => 0,
        ":is_submitted" => 1,
      ];
      \Drupal::database()->query($query, $args);
     $submitted_abstract_id = \Drupal::database()->lastInsertId();
      //var_dump($submitted_abstract_id);die;
      $query1 = "UPDATE esim_circuit_simulation_proposal SET is_submitted = :is_submitted WHERE id = :id";
      $args1 = [
        ":is_submitted" => 1,
        ":id" => $proposal_id,
      ];
      \Drupal::database()->query($query1, $args1);
      \Drupal::messenger()->addStatus('Abstract uploaded successfully.');
    } //!$query_s_result
    else {
      $query = "UPDATE {esim_circuit_simulation_submitted_abstracts} SET
	abstract_upload_date =:abstract_upload_date,
	is_submitted= :is_submitted 
	WHERE proposal_id = :proposal_id
	";
      $args = [
        ":abstract_upload_date" => time(),
        ":is_submitted" => 1,
        ":proposal_id" => $proposal_id,
      ];
      $submitted_abstract_id = \Drupal::database()->query($query, $args);
      $query1 = "UPDATE {esim_circuit_simulation_proposal} SET is_submitted = :is_submitted WHERE id = :id";
      $args1 = [
        ":is_submitted" => 1,
        ":id" => $proposal_id,
      ];
      \Drupal::database()->query($query1, $args1);
      \Drupal::messenger()->addStatus('Abstract updated successfully.');
    }
    foreach ($_FILES['files']['name'] as $file_form_name => $file_name) {
      if ($file_name) {
        /* checking file type */
        if (strstr($file_form_name, 'upload_circuit_simulation_developed_process')) {
          $file_type = "S";
        } //strstr($file_form_name, 'upload_circuit_simulation_developed_process')
        else {
          if (strstr($file_form_name, 'upload_an_abstract')) {
            $file_type = "A";
          }
        }
        /*switch ($file_type) {
          case 'S':*/
            if (file_exists($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name])) {
              //unlink($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]);
              move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]);
              \Drupal::messenger()->addError(t("File @filename already exists hence overwirtten the exisitng file ", [
                '@filename' => $_FILES['files']['name'][$file_form_name]
                ]));
            } //file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
					/* uploading file */
            else {
              if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name])) {
                /* for uploaded files making an entry in the database */
                $query_ab_f = "SELECT * FROM esim_circuit_simulation_submitted_abstracts_file WHERE proposal_id = :proposal_id AND filetype = 
				:filetype";
                $args_ab_f = [
                  ":proposal_id" => $proposal_id,
                  ":filetype" => $file_type,
                ];
                $query_ab_f_result = \Drupal::database()->query($query_ab_f, $args_ab_f)->fetchObject();
                if (!$query_ab_f_result) {
                  $query = "INSERT INTO esim_circuit_simulation_submitted_abstracts_file (submitted_abstract_id, proposal_id, uid, approvar_uid, filename, filepath, filemime, filesize, filetype, timestamp)
          VALUES (:submitted_abstract_id, :proposal_id, :uid, :approvar_uid, :filename, :filepath, :filemime, :filesize, :filetype, :timestamp)";
                  $args = [
                    ":submitted_abstract_id" => $submitted_abstract_id,
                    ":proposal_id" => $proposal_id,
                    ":uid" => $user->id(),
                    ":approvar_uid" => 0,
                    ":filename" => $_FILES['files']['name'][$file_form_name],
                    ":filepath" => $_FILES['files']['name'][$file_form_name],
                    ":filemime" => mime_content_type($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]),
                    ":filesize" => filesize($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]),
                    ":filetype" => $file_type,
                    ":timestamp" => time(),
                  ];
                 // var_dump($args);die;
                 $insert_files = Database::getConnection()->insert('esim_circuit_simulation_submitted_abstracts_file')->fields($args)->execute();
                  \Drupal::messenger()->addStatus($file_name . ' uploaded successfully.');
                } //!$query_ab_f_result
                else {
                  unlink($root_path . $dest_path_project_files . $query_ab_f_result->filename);
                  $query = "UPDATE esim_circuit_simulation_submitted_abstracts_file SET filename = :filename, filepath=:filepath, filemime=:filemime, filesize=:filesize, timestamp=:timestamp WHERE proposal_id = :proposal_id AND filetype = :filetype";
                  $args = [
                    ":filename" => $_FILES['files']['name'][$file_form_name],
                    ":filepath" => $file_path . $_FILES['files']['name'][$file_form_name],
                    ":filemime" => mime_content_type($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]),
                    ":filesize" => $_FILES['files']['size'][$file_form_name],
                    ":timestamp" => time(),
                    ":proposal_id" => $proposal_id,
                    ":filetype" => $file_type,
                  ];
                  \Drupal::database()->query($query, $args);
                  \Drupal::messenger()->addStatus($file_name . ' file updated successfully.');
                }
              } //move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
              else {
                \Drupal::messenger()->addError('Error uploading file : ' . $dest_path_project_files . $file_name);
              }
            } //$file_type
          }
      } //$file_name
	/* sending email */
    $email_to = $user->getEmail();
    $from = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_from_email');
    $bcc = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_emails');
    $cc = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_cc_emails');
    $params['abstract_uploaded']['proposal_id'] = $proposal_id;
    $params['abstract_uploaded']['submitted_abstract_id'] = $submitted_abstract_id;
    $params['abstract_uploaded']['user_id'] = $user->id();
    $params['abstract_uploaded']['headers'] = [
      'From' => $from,
      'MIME-Version' => '1.0',
      'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
      'Content-Transfer-Encoding' => '8Bit',
      'X-Mailer' => 'Drupal',
      'Cc' => $cc,
      'Bcc' => $bcc,
    ];
    /*if (!drupal_mail('circuit_simulation', 'abstract_uploaded', $email_to, language_default(), $params, $from, TRUE)) {
      \Drupal::messenger()->addError('Error sending email message.');
    }*/
    $response = new RedirectResponse(Url::fromUri('internal:/circuit-simulation-project/abstract-code')->toString());
    // Send the redirect response
   // $response->send();
  }
}
?>
