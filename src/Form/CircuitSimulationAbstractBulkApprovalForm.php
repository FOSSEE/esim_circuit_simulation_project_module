<?php

/**
 * @file
 * Contains \Drupal\circuit_simulation\Form\CircuitSimulationAbstractBulkApprovalForm.
 */

namespace Drupal\circuit_simulation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;

class CircuitSimulationAbstractBulkApprovalForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'circuit_simulation_abstract_bulk_approval_form';
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $options_first = _bulk_list_of_circuit_simulation_project();
    $selected = !$form_state->getValue(['circuit_simulation_project']) ? $form_state->getValue([
      'circuit_simulation_project'
      ]) : key($options_first);
    $form = [];
    $form['circuit_simulation_project'] = [
      '#type' => 'select',
      '#title' => t('Title of the circuit simulation project'),
      '#options' => _bulk_list_of_circuit_simulation_project(),
      '#default_value' => $selected,
      '#ajax' => [
        'callback' => 'ajax_bulk_circuit_simulation_abstract_details_callback'
        ],
      '#suffix' => '<div id="ajax_selected_circuit_simulation"></div><div id="ajax_selected_circuit_simulation_pdf"></div>',
    ];
    $form['circuit_simulation_actions'] = [
      '#type' => 'select',
      '#title' => t('Please select action for Circuit Simulation project'),
      '#options' => _bulk_list_circuit_simulation_actions(),
      '#default_value' => 0,
      '#prefix' => '<div id="ajax_selected_circuit_simulation_action" style="color:red;">',
      '#suffix' => '</div>',
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
      '#states' => [
        'invisible' => [
          ':input[name="lab"]' => [
            'value' => 0
            ]
          ]
        ],
    ];
    return $form;
  }

  public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $user = \Drupal::currentUser();
    $msg = '';
    $root_path = circuit_simulation_document_path();
    if ($form_state->get(['clicked_button', '#value']) == 'Submit') {
      if ($form_state->getValue(['circuit_simulation_project']))
        // circuit_simulation_abstract_del_lab_pdf($form_state['values']['circuit_simulation_project']);
 {
        if (\Drupal::currentUser()->hasPermission('esim circuit simulation bulk manage abstract')) {
          $query = \Drupal::database()->select('esim_circuit_simulation_proposal');
          $query->fields('esim_circuit_simulation_proposal');
          $query->condition('id', $form_state->getValue(['circuit_simulation_project']));
          $user_query = $query->execute();
          $user_info = $user_query->fetchObject();
          $user_data = \Drupal::entityTypeManager()->getStorage('user')->load($user_info->uid);
          if ($form_state->getValue(['circuit_simulation_actions']) == 1) {
            // approving entire project //
            $query = \Drupal::database()->select('esim_circuit_simulation_submitted_abstracts');
            $query->fields('esim_circuit_simulation_submitted_abstracts');
            $query->condition('proposal_id', $form_state->getValue(['circuit_simulation_project']));
            $abstracts_q = $query->execute();
            $experiment_list = '';
            while ($abstract_data = $abstracts_q->fetchObject()) {
              \Drupal::database()->query("UPDATE {esim_circuit_simulation_submitted_abstracts} SET abstract_approval_status = 1, is_submitted = 1, approver_uid = :approver_uid WHERE id = :id", [
                ':approver_uid' => $user->uid,
                ':id' => $abstract_data->id,
              ]);
              \Drupal::database()->query("UPDATE {esim_circuit_simulation_submitted_abstracts_file} SET file_approval_status = 1, approvar_uid = :approver_uid WHERE submitted_abstract_id = :submitted_abstract_id", [
                ':approver_uid' => $user->uid,
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
            $email_to = $user_data->mail;
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
              $msg = \Drupal::messenger()->addError('Error sending email message.');
            } //!drupal_mail('circuit_simulation', 'standard', $email_to, language_default(), $params, $from, TRUE)
          } //$form_state['values']['circuit_simulation_actions'] == 1
          elseif ($form_state->getValue(['circuit_simulation_actions']) == 2) {
            if (strlen(trim($form_state->getValue(['message']))) <= 30) {
              $form_state->setErrorByName('message', t(''));
              $msg = \Drupal::messenger()->addError("Please mention the reason for resubmission. Minimum 30 character required");
              return $msg;
            }
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
            $email_to = $user_data->mail;
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
            } //!drupal_mail('circuit_simulation', 'standard', $email_to, language_default(), $params, $from, TRUE)
          } //$form_state['values']['circuit_simulation_actions'] == 2
          elseif ($form_state->getValue(['circuit_simulation_actions']) == 3) //disapprove and delete entire circuit simulation project
 {
            if (strlen(trim($form_state->getValue(['message']))) <= 30) {
              $form_state->setErrorByName('message', t(''));
              $msg = \Drupal::messenger()->addError("Please mention the reason for disapproval. Minimum 30 character required");
              return $msg;
            } //strlen(trim($form_state['values']['message'])) <= 30
            if (!\Drupal::currentUser()->hasPermission('esim circuit simulation bulk delete abstract')) {
              $msg = \Drupal::messenger()->addError(t('You do not have permission to Bulk Dis-Approved and Deleted Entire Lab.'));
              return $msg;
            } //!user_access('circuit_simulation bulk delete code')
            if (circuit_simulation_abstract_delete_project($form_state->getValue(['circuit_simulation_project']))) //////
 {
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

              $email_to = $user_data->mail;
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
              }
            } //circuit_simulation_abstract_delete_project($form_state['values']['circuit_simulation_project'])
            else {
              \Drupal::messenger()->addError(t('Error Dis-Approving and Deleting Entire circuit simulation project.'));
            }
            // email 

          } //$form_state['values']['circuit_simulation_actions'] == 3
				/*elseif ($form_state['values']['circuit_simulation_actions'] == 4)
				{
					if (strlen(trim($form_state['values']['message'])) <= 30)
					{
						form_set_error('message', t(''));
						$msg = drupal_set_message("Please mention the reason for disapproval/deletion. Minimum 30 character required", 'error');
						return $msg;
					} //strlen(trim($form_state['values']['message'])) <= 30
					$query = db_select('esim_circuit_simulation_abstract_experiment');
					$query->fields('esim_circuit_simulation_abstract_experiment');
					$query->condition('proposal_id', $form_state['values']['lab']);
					$query->orderBy('number', 'ASC');
					$experiment_q = $query->execute();
					$experiment_list = '';
					while ($experiment_data = $experiment_q->fetchObject())
					{
						$experiment_list .= '<p>' . $experiment_data->number . ') ' . $experiment_data->title . '<br> Description :  ' . $experiment_data->description . '<br>';
						$experiment_list .= ' ';
						$experiment_list .= '</p>';
					} //$experiment_data = $experiment_q->fetchObject()
					if (!user_access('lab migration bulk delete code'))
					{
						$msg = drupal_set_message(t('You do not have permission to Bulk Delete Entire Lab Including Proposal.'), 'error');
						return $msg;
					} //!user_access('lab migration bulk delete code')
					// check if dependency files are present 
					$dep_q = db_query("SELECT * FROM {esim_circuit_simulation_abstract_dependency_files} WHERE proposal_id = :proposal_id", array(
						":proposal_id" => $form_state['values']['lab']
					));
					if ($dep_data = $dep_q->fetchObject())
					{
						$msg = drupal_set_message(t("Cannot delete lab since it has dependency files that can be used by others. First delete the dependency files before deleting the lab."), 'error');
						return $msg ;
					} //$dep_data = $dep_q->fetchObject()
					if (circuit_simulation_abstract_delete_lab($form_state['values']['lab']))
					{
						drupal_set_message(t('Dis-Approved and Deleted Entire Lab solutions.'), 'status');
						$query = db_select('esim_circuit_simulation_abstract_experiment');
						$query->fields('esim_circuit_simulation_abstract_experiment');
						$query->condition('proposal_id', $form_state['values']['lab']);
						$experiment_q = $query->execute()->fetchObject();
						$dir_path = $root_path . $experiment_q->directory_name;
						if (is_dir($dir_path))
						{
							$res = rmdir($dir_path);
							if (!$res)
							{
								$msg = drupal_set_message(t("Cannot delete Lab directory : " . $dir_path . ". Please contact administrator."), 'error');
								return $msg;
							} //!$res
						} //is_dir($dir_path)
						else
						{
							drupal_set_message(t("Lab directory not present : " . $dir_path . ". Skipping deleting lab directory."), 'status');
						}
						$proposal_q = db_query("SELECT * FROM {esim_circuit_simulation_abstract_proposal} WHERE id = :id", array(
							":id" => $form_state['values']['lab']
						));
						$proposal_data = $proposal_q->fetchObject();
						$proposal_id = $proposal_data->id;
						db_query("DELETE FROM {esim_circuit_simulation_abstract_experiment} WHERE proposal_id = :proposal_id", array(
							":proposal_id" => $proposal_id
						));
						db_query("DELETE FROM {esim_circuit_simulation_abstract_proposal} WHERE id = :id", array(
							":id" => $proposal_id
						));
						drupal_set_message(t('Deleted Lab Proposal.'), 'status');
						//email 
						$email_subject = t('[!site_name] Your uploaded Lab Migration solutions including the Lab proposal have been deleted', array(
							'!site_name' => variable_get('site_name', '')
						));
						$email_body = array(
							0 => t('

Dear !user_name,

We regret to inform you that all the uploaded Experiments of your Lab with following details have been deleted permanently.

Title of Lab :' . $user_info->lab_title . '

List of experiments : ' . $experiment_list . '

Reason for dis-approval: ' . $form_state['values']['message'] . '

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
								'!site_name' => variable_get('site_name', ''),
								'!user_name' => $user_data->name
							))
						);
						// email 
						//  $email_subject = t('Your uploaded Lab Migration solutions including the Lab proposal have been deleted');
						$email_body = array(
							0 => t('Your all the uploaded solutions including the Lab proposal have been deleted permanently.')
						);
					} //circuit_simulation_abstract_delete_lab($form_state['values']['lab'])
					else
					{
						$msg = drupal_set_message(t('Error Dis-Approving and Deleting Entire Lab.'), 'error');
					}
				} //$form_state['values']['circuit_simulation_actions'] == 4
				else
				{
					$msg = drupal_set_message(t('You do not have permission to bulk manage code.'), 'error');
				}*/
        }
      } //user_access('circuit_simulation project bulk manage code')
      return $msg;
    } //$form_state['clicked_button']['#value'] == 'Submit'
  }

}
?>
