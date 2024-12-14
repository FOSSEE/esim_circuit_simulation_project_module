<?php

/**
 * @file
 * Contains \Drupal\circuit_simulation\Form\CircuitSimulationAbstractBulkApprovalForm.
 */

namespace Drupal\circuit_simulation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;
use Drupal\Core\Url;
use Drupal\Core\Link;
use Drupal\Core\Database\Database;
use Drupal\Core\Ajax\AjaxResponse;
use Drupal\Core\Ajax\HtmlCommand;
use Drupal\Core\Ajax\ReplaceCommand;
use Drupal\Core\Routing\TrustedRedirectResponse;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\RedirectResponse;


class CircuitSimulationAbstractBulkApprovalForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'circuit_simulation_abstract_bulk_approval_form';
  }
  function _bulk_list_of_circuit_simulation_project() {
  $project_titles = [
    0 => 'Please select...'
  ];

  // Access the database connection.
  $connection = Database::getConnection();

  // Build the query.
  $query = $connection->select('esim_circuit_simulation_proposal', 'e')
    ->fields('e')
    ->condition('is_submitted', 1)
    ->condition('approval_status', 1)
    ->orderBy('project_title', 'ASC');

  // Execute the query and fetch the results.
  $results = $query->execute();

  foreach ($results as $record) {
    $project_titles[$record->id] = $record->project_title . ' (Proposed by ' . $record->contributor_name . ')';
  }

  return $project_titles;
}
function _bulk_list_circuit_simulation_actions() {
  // Define the actions as an associative array.
  $circuit_simulation_actions = [
    0 => 'Please select...',
    1 => 'Approve Entire Circuit Simulation Project',
    2 => 'Resubmit Project files',
    3 => 'Disapprove Entire Circuit Simulation Project (This will delete Circuit Simulation Project)',
    // Uncomment the following line if needed in the future:
    // 4 => 'Delete Entire Circuit Simulation Project Including Proposal',
  ];

  return $circuit_simulation_actions;
}

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $options_first = $this->_bulk_list_of_circuit_simulation_project();
    $selected = !$form_state->getValue(['circuit_simulation_project']) ? $form_state->getValue(['circuit_simulation_project']) : key($options_first);
    $form = [];
    $form['circuit_simulation_project'] = [
      '#type' => 'select',
      '#title' => t('Title of the circuit simulation project'),
      '#options' => $this->_bulk_list_of_circuit_simulation_project(),
      '#default_value' => $selected,
      '#ajax' => [
        'callback' => '::ajax_bulk_circuit_simulation_abstract_details_callback',
        'wrapper' => 'ajax_selected_circuit_simulation'
        ],
      // '#suffix' => '<div id="ajax_selected_circuit_simulation"></div><div id="ajax_selected_circuit_simulation_pdf"></div>',
    ];
    $form['update_circuit_simulation'] = [
      '#type' => 'container',
      '#attributes' => ['id' => 'ajax_selected_circuit_simulation'],
      '#states' => [
        'invisible' => [
          ':input[name="circuit_simulation_project"]' => [
            'value' => 0
            ]
          ]
        ],
    ];
    $form['update_circuit_simulation']['cs_details'] = [
      '#type' => 'markup',
      '#markup' => $this->_circuit_simulation_details($form_state->getValue('circuit_simulation_project')),
      '#states' => [
        'invisible' => [
          ':input[name="circuit_simulation_project"]' => [
            'value' => 0
            ]
          ]
        ],
    ];
    $form['circuit_simulation_actions'] = [
      '#type' => 'select',
      '#title' => t('Please select action for Circuit Simulation project'),
      '#options' => $this->_bulk_list_circuit_simulation_actions(),
      '#default_value' => 0,
      '#states' => [
        'invisible' => [
          ':input[name="circuit_simulation_project"]' => [
            'value' => 0
            ]
          ]
        ],
    ];
    $form['message'] = [
      '#type' => 'textarea',
      '#title' => t('Please specify the reason for Resubmission / Dis-Approval'),
      '#prefix' => '<div id= "message_submit">',
      '#states' => [
        'visible' => [
          [
            ':input[name="circuit_simulation_actions"]' => [
              'value' => 3
              ]
            ],
          'or',
          [
            ':input[name="circuit_simulation_actions"]' => [
              'value' => 2
              ]
            ],
        ]
        ],
    ];
    $form['submit'] = [
      '#type' => 'submit',
      '#value' => t('Submit'),
      /*'#states' => [
        'invisible' => [
          ':input[name="circuit_simulation_project"]' => [
            'value' => 0
            ]
          ]
        ],*/
    ];
    return $form;
  }
  function ajax_bulk_circuit_simulation_abstract_details_callback(array &$form, FormStateInterface $form_state) {
    return $form['update_circuit_simulation'];
}
  function _circuit_simulation_details($circuit_simulation_proposal_id) {
//var_dump($circuit_simulation_proposal_id);die;
  $return_html = '';

  // Get the proposal details.
  $connection = Database::getConnection();

  $query_pro = $connection->select('esim_circuit_simulation_proposal', 'p')
    ->fields('p')
    ->condition('id', $circuit_simulation_proposal_id);
  $abstracts_pro = $query_pro->execute()->fetchObject();

  // Get the abstract PDF file.
  $query_pdf = $connection->select('esim_circuit_simulation_submitted_abstracts_file', 'f')
    ->fields('f')
    ->condition('proposal_id', $circuit_simulation_proposal_id)
    ->condition('filetype', 'A');
  $abstracts_pdf = $query_pdf->execute()->fetchObject();

  $abstract_filename = 'File not uploaded';
  if ($abstracts_pdf && !empty($abstracts_pdf->filename) && $abstracts_pdf->filename !== 'NULL') {
    $abstract_filename = $abstracts_pdf->filename;
  }

  // Get the circuit simulation process file.
  $query_process = $connection->select('esim_circuit_simulation_submitted_abstracts_file', 'f')
    ->fields('f')
    ->condition('proposal_id', $circuit_simulation_proposal_id)
    ->condition('filetype', 'S');
  $abstracts_query_process = $query_process->execute()->fetchObject();

  $abstracts_query_process_filename = 'File not uploaded';
  if ($abstracts_query_process && !empty($abstracts_query_process->filename) && $abstracts_query_process->filename !== 'NULL') {
    $abstracts_query_process_filename = $abstracts_query_process->filename;
  } 

  // Get additional abstract submission details.
  $query = $connection->select('esim_circuit_simulation_submitted_abstracts', 'a')
    ->fields('a')
    ->condition('proposal_id', $circuit_simulation_proposal_id);
  $abstracts_q = $query->execute()->fetchObject();

  if ($abstracts_q && $abstracts_q->is_submitted == 0) {
    // Optional message if the abstract is not submitted.
    // drupal_set_message($this->t('Abstract is not submitted yet.'), 'error');
  }

  // Download link for the circuit simulation project.
  $download_url = Url::fromUri('internal:/circuit-simulation-project/full-download/project/' . $circuit_simulation_proposal_id);
  $download_circuit_simulation = Link::fromTextAndUrl('Download circuit simulation project', $download_url)->toString();

  // Build the HTML output.
  $return_html .= '<strong>Proposer Name:</strong><br />' . htmlspecialchars($abstracts_pro->name_title . ' ' . $abstracts_pro->contributor_name) . '<br /><br />';
  $return_html .= '<strong>Title of the Circuit Simulation Project:</strong><br />' . $abstracts_pro->project_title . '<br /><br />';
  $return_html .= '<strong>Uploaded an abstract (brief outline) of the project:</strong><br />' . $abstract_filename . '<br /><br />';
  $return_html .= '<strong>Upload the eSim circuit simulation for the developed process:</strong><br />' . $abstracts_query_process_filename . '<br /><br />';
  $return_html .= $download_circuit_simulation;

  return $return_html;
}
  
  public function validateForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    if ($form_state->getValue(['circuit_simulation_actions']) == 2 || $form_state->getValue(['circuit_simulation_actions']) == 3){
    if (strlen(trim($form_state->getValue(['message']))) <= 30) {
              $form_state->setErrorByName('message', t('Please mention the reason for resubmission or disapproval. Minimum 30 character required'));
              /*$msg = \Drupal::messenger()->addError("");
              return $msg;*/
            }
          }
  }
  public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $service = \Drupal::service('circuit_simulation_global');
    $user = \Drupal::currentUser();
    $msg = '';
    $root_path = $service->circuit_simulation_path();
    //if ($form_state->get(['clicked_button', '#value']) == 'Submit') {
      if ($form_state->getValue(['circuit_simulation_project']))
        // circuit_simulation_abstract_del_lab_pdf($form_state['values']['circuit_simulation_project']);
        {
          //var_dump($form_state->getValue(['circuit_simulation_actions']));die;
        if (\Drupal::currentUser()->hasPermission('esim circuit simulation bulk manage abstract')) 
          {
          $query = \Drupal::database()->select('esim_circuit_simulation_proposal');
          $query->fields('esim_circuit_simulation_proposal');
          $query->condition('id', $form_state->getValue(['circuit_simulation_project']));
          $user_query = $query->execute();
          $user_info = $user_query->fetchObject();
          $user_data = \Drupal::entityTypeManager()->getStorage('user')->load($user_info->uid);
          if ($form_state->getValue(['circuit_simulation_actions']) == 1) {
            // approving entire project //
            //var_dump("hi");die;
            $query = \Drupal::database()->select('esim_circuit_simulation_submitted_abstracts');
            $query->fields('esim_circuit_simulation_submitted_abstracts');
            $query->condition('proposal_id', $form_state->getValue(['circuit_simulation_project']));
            $abstracts_q = $query->execute();
            $experiment_list = '';
            while ($abstract_data = $abstracts_q->fetchObject()) {
              \Drupal::database()->query("UPDATE {esim_circuit_simulation_submitted_abstracts} SET abstract_approval_status = 1, is_submitted = 1, approver_uid = :approver_uid WHERE id = :id", [
                ':approver_uid' => $user->id(),
                ':id' => $abstract_data->id,
              ]);
              \Drupal::database()->query("UPDATE {esim_circuit_simulation_submitted_abstracts_file} SET file_approval_status = 1, approvar_uid = :approver_uid WHERE submitted_abstract_id = :submitted_abstract_id", [
                ':approver_uid' => $user->id(),
                ':submitted_abstract_id' => $abstract_data->id,
              ]);
            } //$abstract_data = $abstracts_q->fetchObject()
            \Drupal::messenger()->addStatus(t('Approved Circuit Simulation project.'));
            // email 
            // @FIXME
            // // @FIXME
            // // This looks like another module's variable. You'll need to rewrite this call
            // // to ensure that it uses the correct configuration object.
            // $email_subject = t('[!site_name][Circuit Simulation Project] Your uploaded circuit simulation project have been approved', array(
            // 						'!site_name' => variable_get('site_name', '')
            // 					));

            // @FIXME
            // // @FIXME
            // // This looks like another module's variable. You'll need to rewrite this call
            // // to ensure that it uses the correct configuration object.
            // $email_body = array(
            // 						0 => t('
            // 
            // Dear !user_name,
            // 
            // Your uploaded abstract for the circuit simulation project has been approved:
            // 
            // Title of circuit simulation project  : ' . $user_info->project_title . '
            // 
            // Best Wishes,
            // 
            // !site_name Team,
            // FOSSEE,IIT Bombay', array(
            // 							'!site_name' => variable_get('site_name', ''),
            // 							'!user_name' => $user_data->name
            // 						))
            // 					);

            /** sending email when everything done **/
            /*$email_to = $user_data->getEmail();
            $from = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_from_email');
            $bcc = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_emails');
            $cc = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_cc_emails');
            $params['standard']['subject'] = $email_subject;
            $params['standard']['body'] = $email_body;
            $params['standard']['headers'] = [
              'From' => $from,
              'MIME-Version' => '1.0',
              'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
              'Content-Transfer-Encoding' => '8Bit',
              'X-Mailer' => 'Drupal',
              'Cc' => $cc,
              'Bcc' => $bcc,
            ];
            if (!drupal_mail('circuit_simulation', 'standard', $email_to, language_default(), $params, $from, TRUE)) {
              $msg = \Drupal::messenger()->addError('Error sending email message.');*/
              $response = new RedirectResponse(Url::fromUri('internal:/circuit-simulation-project/manage-proposal/status/' . $form_state->getValue(['circuit_simulation_project']))->toString());
              $response->send();
            }//$form_state['values']['circuit_simulation_actions'] == 1
          else if ($form_state->getValue(['circuit_simulation_actions']) == 2) 
          {
            //pending review entire project 
            $query = \Drupal::database()->select('esim_circuit_simulation_submitted_abstracts');
            $query->fields('esim_circuit_simulation_submitted_abstracts');
            $query->condition('proposal_id', $form_state->getValue(['circuit_simulation_project']));
            $abstracts_q = $query->execute();
            $experiment_list = '';
            while ($abstract_data = $abstracts_q->fetchObject()) {
              \Drupal::database()->query("UPDATE {esim_circuit_simulation_submitted_abstracts} SET abstract_approval_status = 0, is_submitted = 0, approver_uid = :approver_uid WHERE id = :id", [
                ':approver_uid' => $user->uid,
                ':id' => $abstract_data->id,
              ]);
              \Drupal::database()->query("UPDATE {esim_circuit_simulation_proposal} SET is_submitted = 0, approver_uid = :approver_uid WHERE id = :id", [
                ':approver_uid' => $user->uid,
                ':id' => $abstract_data->proposal_id,
              ]);
              \Drupal::database()->query("UPDATE {esim_circuit_simulation_submitted_abstracts_file} SET file_approval_status = 0, approvar_uid = :approver_uid WHERE submitted_abstract_id = :submitted_abstract_id", [
                ':approver_uid' => $user->uid,
                ':submitted_abstract_id' => $abstract_data->id,
              ]);
            } //$abstract_data = $abstracts_q->fetchObject()
            \Drupal::messenger()->addStatus(t('Resubmit the project files'));
            // email 
            // @FIXME
            // // @FIXME
            // // This looks like another module's variable. You'll need to rewrite this call
            // // to ensure that it uses the correct configuration object.
            // $email_subject = t('[!site_name][Circuit Simulation Project] Your uploaded circuit simulation project have been marked as pending', array(
            // 						'!site_name' => variable_get('site_name', '')
            // 					));

            // @FIXME
            // // @FIXME
            // // This looks like another module's variable. You'll need to rewrite this call
            // // to ensure that it uses the correct configuration object.
            // $email_body = array(
            // 						0 => t('
            // 
            // Dear !user_name,
            // 
            // Kindly resubmit the project files for the project : ' . $user_info->project_title . 'after making changes considering the following reviewer’s comments.
            // 
            // Comment: ' . $form_state['values']['message'] . '
            // 
            // Best Wishes,
            // 
            // !site_name Team,
            // FOSSEE, IIT Bombay', array(
            // 							'!site_name' => variable_get('site_name', ''),
            // 							'!user_name' => $user_data->name
            // 						))
            // 					);

            /** sending email when everything done **/
            /*$email_to = $user_data->getEmail();
            $from = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_from_email');
            $bcc = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_emails');
            $cc = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_cc_emails');
            $params['standard']['subject'] = $email_subject;
            $params['standard']['body'] = $email_body;
            $params['standard']['headers'] = [
              'From' => $from,
              'MIME-Version' => '1.0',
              'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
              'Content-Transfer-Encoding' => '8Bit',
              'X-Mailer' => 'Drupal',
              'Cc' => $cc,
              'Bcc' => $bcc,
            ];
            if (!drupal_mail('circuit_simulation', 'standard', $email_to, language_default(), $params, $from, TRUE)) {
              \Drupal::messenger()->addError('Error sending email message.');*/
            } //!drupal_mail('circuit_simulation', 'standard', $email_to, language_default(), $params, $from, TRUE)
           //$form_state['values']['circuit_simulation_actions'] == 2
          else if ($form_state->getValue(['circuit_simulation_actions']) == 3) //disapprove and delete entire circuit simulation project
          {
            //var_dump('hi');die;
            // if (strlen(trim($form_state->getValue(['message']))) <= 30) {
            //   $form_state->setErrorByName('message', t(''));
            //   $msg = \Drupal::messenger()->addError("Please mention the reason for disapproval. Minimum 30 character required");
            //   return $msg;
            // } //strlen(trim($form_state['values']['message'])) <= 30
            if (!\Drupal::currentUser()->hasPermission('esim circuit simulation bulk delete abstract')) {
              $msg = \Drupal::messenger()->addError(t('You do not have permission to Bulk Dis-Approved and Deleted Entire Lab.'));
              return $msg;
            } //!user_access('circuit_simulation bulk delete code')
            if ($service->circuit_simulation_abstract_delete_project($form_state->getValue(['circuit_simulation_project']))) //////
             {
             // var_dump('hi');die;
              \Drupal::messenger()->addStatus(t('Dis-Approved and Deleted Entire Circuit Simulation project.'));
              // @FIXME
              // // @FIXME
              // // This looks like another module's variable. You'll need to rewrite this call
              // // to ensure that it uses the correct configuration object.
              // $email_subject = t('[!site_name][Circuit Simulation Project] Your uploaded circuit simulation project have been marked as dis-approved', array(
              // 						'!site_name' => variable_get('site_name', '')
              // 					));

              // @FIXME
              // // @FIXME
              // // This looks like another module's variable. You'll need to rewrite this call
              // // to ensure that it uses the correct configuration object.
              // $email_body = array(
              // 						0 => t('
              // 
              // Dear !user_name,
              // 
              // Your uploaded circuit simulation project files for the circuit simulation project Title : ' . $user_info->project_title . ' have been marked as dis-approved.
              // 
              // Reason for dis-approval: ' . $form_state['values']['message'] . '
              // 
              // Best Wishes,
              // 
              // !site_name Team,
              // FOSSEE, IIT Bombay', array(
              // 					'!site_name' => variable_get('site_name', ''),
              // 					'!user_name' => $user_data->name
              // 											))
              // 					);

              /*$email_to = $user_data->getEmail();
              $from = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_from_email');
              $bcc = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_emails');
              $cc = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_cc_emails');
              $params['standard']['subject'] = $email_subject;
              $params['standard']['body'] = $email_body;
              $params['standard']['headers'] = [
                'From' => $from,
                'MIME-Version' => '1.0',
                'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
                'Content-Transfer-Encoding' => '8Bit',
                'X-Mailer' => 'Drupal',
                'Cc' => $cc,
                'Bcc' => $bcc,
              ];
              if (!drupal_mail('circuit_simulation', 'standard', $email_to, language_default(), $params, $from, TRUE)) {
                \Drupal::messenger()->addError('Error sending email message.');
              }*/
            } //circuit_simulation_abstract_delete_project($form_state['values']['circuit_simulation_project'])
            else {
              \Drupal::messenger()->addError(t('Error Dis-Approving and Deleting Entire circuit simulation project.'));
            }
            // email 

          } //$form_state['values']['circuit_simulation_actions'] == 3
      }
      } //user_access('circuit_simulation project bulk manage code')
      return $msg;
    } //$form_state['clicked_button']['#value'] == 'Submit'

}
?>
