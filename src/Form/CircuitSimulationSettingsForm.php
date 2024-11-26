<?php

/**
 * @file
 * Contains \Drupal\circuit_simulation\Form\CircuitSimulationSettingsForm.
 */

namespace Drupal\circuit_simulation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;

class CircuitSimulationSettingsForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'circuit_simulation_settings_form';
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $form['emails'] = [
      '#type' => 'textfield',
      '#title' => t('(Bcc) Notification emails'),
      '#description' => t('Specify emails id for Bcc option of mail system with comma separated'),
      '#size' => 50,
      '#maxlength' => 255,
      '#required' => TRUE,
      '#default_value' => \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_emails'),
    ];
    $form['cc_emails'] = [
      '#type' => 'textfield',
      '#title' => t('(Cc) Notification emails'),
      '#description' => t('Specify emails id for Cc option of mail system with comma separated'),
      '#size' => 50,
      '#maxlength' => 255,
      '#required' => TRUE,
      '#default_value' => \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_cc_emails'),
    ];
    $form['from_email'] = [
      '#type' => 'textfield',
      '#title' => t('Outgoing from email address'),
      '#description' => t('Email address to be display in the from field of all outgoing messages'),
      '#size' => 50,
      '#maxlength' => 255,
      '#required' => TRUE,
      '#default_value' => \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_from_email'),
    ];
    
    $form['extensions']['resource_upload'] = array(
    		'#type' => 'textfield',
    		'#title' => t('Allowed file extensions for uploading resource files'),
    		'#description' => t('A comma separated list WITHOUT SPACE of source file extensions that are permitted to be uploaded on the server'),
    		'#size' => 50,
    		'#maxlength' => 255,
    		'#required' => TRUE,
    		'#default_value' => \Drupal::config('circuit_simulation.settings')->get('resource_upload_extensions')
    	);

    $form['extensions']['abstract_upload'] = [
      '#type' => 'textfield',
      '#title' => t('Allowed file extensions for abstract'),
      '#description' => t('A comma separated list WITHOUT SPACE of pdf file extensions that are permitted to be uploaded on the server'),
      '#size' => 50,
      '#maxlength' => 255,
      '#required' => TRUE,
      '#default_value' => \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_abstract_upload_extensions'),
    ];
    $form['extensions']['circuit_simulation_upload'] = [
      '#type' => 'textfield',
      '#title' => t('Allowed extensions for project files'),
      '#description' => t('A comma separated list WITHOUT SPACE of pdf file extensions that are permitted to be uploaded on the server'),
      '#size' => 50,
      '#maxlength' => 255,
      '#required' => TRUE,
      '#default_value' => \Drupal::config('circuit_simulation.settings')->get('circuit_simulation_project_files_extensions'),
    ];
    $form['submit'] = [
      '#type' => 'submit',
      '#value' => t('Submit'),
    ];
    return $form;
  }

  public function validateForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    return;
  }

  public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    \Drupal::configFactory()->getEditable('circuit_simulation.settings')->set('circuit_simulation_emails', $form_state->getValue(['emails']))->save();
    \Drupal::configFactory()->getEditable('circuit_simulation.settings')->set('circuit_simulation_cc_emails', $form_state->getValue(['cc_emails']))->save();
    \Drupal::configFactory()->getEditable('circuit_simulation.settings')->set('circuit_simulation_from_email', $form_state->getValue(['from_email']))->save();
    \Drupal::configFactory()->getEditable('circuit_simulation.settings')->set('resource_upload_extensions', $form_state->getValue(['resource_upload']))->save();
    \Drupal::configFactory()->getEditable('circuit_simulation.settings')->set('circuit_simulation_abstract_upload_extensions', $form_state->getValue(['abstract_upload']))->save();
    \Drupal::configFactory()->getEditable('circuit_simulation.settings')->set('circuit_simulation_project_files_extensions', $form_state->getValue(['circuit_simulation_upload']))->save();
    \Drupal::messenger()->addStatus(t('Settings updated'));
  }

}
?>
