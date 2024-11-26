<?php

/**
 * @file
 * Contains \Drupal\circuit_simulation\Form\CircuitSimulationCompletedTabForm.
 */

namespace Drupal\circuit_simulation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;

class CircuitSimulationCompletedTabForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'circuit_simulation_completed_tab_form';
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $options_first = _circuit_simulation_details_year_wise();
    $selected = !$form_state->getValue(['howmany_select']) ? $form_state->getValue(['howmany_select']) : key($options_first);
    $form = [];
    $form['howmany_select'] = [
      '#title' => t('Sorting projects according to year:'),
      '#type' => 'select',
      '#options' => _circuit_simulation_details_year_wise(),
      /*'#options' => array(
    	'Please select...' => 'Please select...',
    	'2017' => '2017',
    	'2018' => '2018', 
    	'2019' => '2019', 
    	'2020' => '2020', 
    	'2021' => '2021'),*/
      '#default_value' => $selected,
      '#ajax' => [
        'callback' => 'ajax_example_autocheckboxes_callback'
        ],
      '#suffix' => '<div id="ajax-selected-circuit_simulation"></div>',
    ];
    return $form;
  }
public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
}
}
?>
