<?php

/**
 * @file
 * Contains \Drupal\circuit_simulation\Form\CircuitSimulationProposalApprovalForm.
 */

namespace Drupal\circuit_simulation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;

class CircuitSimulationProposalApprovalForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'circuit_simulation_proposal_approval_form';
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $user = \Drupal::currentUser();
    /* get current proposal */
    $proposal_id = (int) arg(3);
    $query = \Drupal::database()->select('esim_circuit_simulation_proposal');
    $query->fields('esim_circuit_simulation_proposal');
    $query->condition('id', $proposal_id);
    $proposal_q = $query->execute();
    if ($proposal_q) {
      if ($proposal_data = $proposal_q->fetchObject()) {
        /* everything ok */
      } //$proposal_data = $proposal_q->fetchObject()
      else {
        \Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
        drupal_goto('circuit-simulation-project/manage-proposal');
        return;
      }
    } //$proposal_q
    else {
      \Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
      drupal_goto('circuit-simulation-project/manage-proposal');
      return;
    }
    if ($proposal_data->project_guide_name == "NULL" || $proposal_data->project_guide_name == "") {
      $project_guide_name = "Not Entered";
    } //$proposal_data->project_guide_name == NULL
    else {
      $project_guide_name = $proposal_data->project_guide_name;
    }
    if ($proposal_data->project_guide_email_id == "NULL" || $proposal_data->project_guide_email_id == "") {
      $project_guide_email_id = "Not Entered";
    } //$proposal_data->project_guide_email_id == NULL
    else {
      $project_guide_email_id = $proposal_data->project_guide_email_id;
    }
    // @FIXME
    // l() expects a Url object, created from a route name or external URI.
    // $form['contributor_name'] = array(
    // 		'#type' => 'item',
    // 		'#markup' => l($proposal_data->name_title . ' ' . $proposal_data->contributor_name, 'user/' . $proposal_data->uid),
    // 		'#title' => t('Student name')
    // 	);

    $form['student_email_id'] = [
      '#title' => t('Student Email'),
      '#type' => 'item',
      '#markup' => \Drupal::entityTypeManager()->getStorage('user')->load($proposal_data->uid)->mail,
      '#title' => t('Email'),
    ];
    $form['contributor_contact_no'] = [
      '#title' => t('Contact No.'),
      '#type' => 'item',
      '#markup' => $proposal_data->contact_no,
    ];
    $form['university'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->university,
      '#title' => t('University/Institute'),
    ];
    $form['country'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->country,
      '#title' => t('Country'),
    ];
    $form['all_state'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->state,
      '#title' => t('State'),
    ];
    $form['city'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->city,
      '#title' => t('City'),
    ];
    $form['pincode'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->pincode,
      '#title' => t('Pincode/Postal code'),
    ];
    $form['project_guide_name'] = [
      '#type' => 'item',
      '#title' => t('Project guide'),
      '#markup' => $project_guide_name,
    ];
    $form['project_guide_email_id'] = [
      '#type' => 'item',
      '#title' => t('Project guide email'),
      '#markup' => $project_guide_email_id,
    ];
    $form['operating_system'] = [
      '#type' => 'item',
      '#title' => t('Operating System'),
      '#markup' => $proposal_data->operating_system,
    ];
    $form['project_title'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->project_title,
      '#title' => t('Title of the Circuit Simulation Project'),
    ];
    $form['description'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->description,
      '#title' => t('Description of the Circuit Simulation Project'),
    ];
    if (($proposal_data->samplefilepath != "") && ($proposal_data->samplefilepath != 'NULL')) {
      $str = substr($proposal_data->samplefilepath, strrpos($proposal_data->samplefilepath, '/'));
      $resource_file = ltrim($str, '/');

      // @FIXME
      // l() expects a Url object, created from a route name or external URI.
      // $form['samplefilepath'] = array(
      // 			'#type' => 'item',
      // 			'#title' => t('Resource file '),
      // 			'#markup' => l($resource_file, 'circuit-simulation-project/download/resource-file/' . $proposal_id) . ""
      // 		);

    } //$proposal_data->user_defined_compound_filepath != ""
    else {
      $form['samplefilepath'] = [
        '#type' => 'item',
        '#title' => t('Resource file '),
        '#markup' => "Not uploaded<br><br>",
      ];
    }
    $form['approval'] = [
      '#type' => 'radios',
      '#title' => t('eSim circuit-simulation proposal'),
      '#options' => [
        '1' => 'Approve',
        '2' => 'Disapprove',
      ],
      '#required' => TRUE,
    ];
    $form['message'] = [
      '#type' => 'textarea',
      '#title' => t('Reason for disapproval'),
      '#attributes' => [
        'placeholder' => t('Enter reason for disapproval in minimum 30 characters '),
        'cols' => 50,
        'rows' => 4,
      ],
      '#states' => [
        'visible' => [
          ':input[name="approval"]' => [
            'value' => '2'
            ]
          ]
        ],
    ];
    $form['submit'] = [
      '#type' => 'submit',
      '#value' => t('Submit'),
    ];
    // @FIXME
    // l() expects a Url object, created from a route name or external URI.
    // $form['cancel'] = array(
    // 		'#type' => 'item',
    // 		'#markup' => l(t('Cancel'), 'circuit-simulation-project/manage-proposal')
    // 	);

    return $form;
  }

  public function validateForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    if ($form_state->getValue(['approval']) == 2) {
      if ($form_state->getValue(['message']) == '') {
        $form_state->setErrorByName('message', t('Reason for disapproval could not be empty'));
      } //$form_state['values']['message'] == ''
    } //$form_state['values']['approval'] == 2
  }

  public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $user = \Drupal::currentUser();
    /* get current proposal */
    $proposal_id = (int) arg(3);
    $query = \Drupal::database()->select('esim_circuit_simulation_proposal');
    $query->fields('esim_circuit_simulation_proposal');
    $query->condition('id', $proposal_id);
    $proposal_q = $query->execute();
    if ($proposal_q) {
      if ($proposal_data = $proposal_q->fetchObject()) {
        /* everything ok */
      } //$proposal_data = $proposal_q->fetchObject()
      else {
        \Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
        drupal_goto('circuit-simulation-project/manage-proposal');
        return;
      }
    } //$proposal_q
    else {
      \Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
      drupal_goto('circuit-simulation-project/manage-proposal');
      return;
    }
    if ($form_state->getValue(['approval']) == 1) {
      $query = "UPDATE {esim_circuit_simulation_proposal} SET approver_uid = :uid, approval_date = :date, approval_status = 1 WHERE id = :proposal_id";
      $args = [
        ":uid" => $user->uid,
        ":date" => time(),
        ":proposal_id" => $proposal_id,
      ];
      \Drupal::database()->query($query, $args);
      /* sending email */
      $user_data = \Drupal::entityTypeManager()->getStorage('user')->load($proposal_data->uid);
      $email_to = $user_data->mail;
      $from = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_from_email');
      $bcc = $user->mail . ', ' . \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_emails');
      $cc = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_cc_emails');
      $params['circuit_simulation_proposal_approved']['proposal_id'] = $proposal_id;
      $params['circuit_simulation_proposal_approved']['user_id'] = $proposal_data->uid;
      $params['circuit_simulation_proposal_approved']['headers'] = [
        'From' => $from,
        'MIME-Version' => '1.0',
        'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
        'Content-Transfer-Encoding' => '8Bit',
        'X-Mailer' => 'Drupal',
        'Cc' => $cc,
        'Bcc' => $bcc,
      ];
      if (!drupal_mail('circuit_simulation', 'circuit_simulation_proposal_approved', $email_to, language_default(), $params, $from, TRUE)) {
        \Drupal::messenger()->addError('Error sending email message.');
      }
      \Drupal::messenger()->addStatus('eSim circuit-simulation proposal No. ' . $proposal_id . ' approved. User has been notified of the approval.');
      drupal_goto('circuit-simulation-project/manage-proposal');
      return;
    } //$form_state['values']['approval'] == 1
    else {
      if ($form_state->getValue(['approval']) == 2) {
        $query = "UPDATE {esim_circuit_simulation_proposal} SET approver_uid = :uid, approval_date = :date, approval_status = 2, dissapproval_reason = :dissapproval_reason WHERE id = :proposal_id";
        $args = [
          ":uid" => $user->uid,
          ":date" => time(),
          ":dissapproval_reason" => $form_state->getValue(['message']),
          ":proposal_id" => $proposal_id,
        ];
        $result = \Drupal::database()->query($query, $args);
        /* sending email */
        $user_data = \Drupal::entityTypeManager()->getStorage('user')->load($proposal_data->uid);
        $email_to = $user_data->mail;
        $from = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_from_email');
        $bcc = $user->mail . ', ' . \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_emails');
        $cc = \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_cc_emails');
        $params['circuit_simulation_proposal_disapproved']['proposal_id'] = $proposal_id;
        $params['circuit_simulation_proposal_disapproved']['user_id'] = $proposal_data->uid;
        $params['circuit_simulation_proposal_disapproved']['headers'] = [
          'From' => $from,
          'MIME-Version' => '1.0',
          'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
          'Content-Transfer-Encoding' => '8Bit',
          'X-Mailer' => 'Drupal',
          'Cc' => $cc,
          'Bcc' => $bcc,
        ];
        if (!drupal_mail('circuit_simulation', 'circuit_simulation_proposal_disapproved', $email_to, language_default(), $params, $from, TRUE)) {
          \Drupal::messenger()->addError('Error sending email message.');
        }
        \Drupal::messenger()->addError('eSim circuit simulation proposal No. ' . $proposal_id . ' dis-approved. User has been notified of the dis-approval.');
        drupal_goto('circuit-simulation-project/manage-proposal');
        return;
      }
    } //$form_state['values']['approval'] == 2
  }

}
?>
