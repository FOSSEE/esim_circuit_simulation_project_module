<?php

/**
 * @file
 * Contains \Drupal\circuit_simulation\Form\CircuitSimulationRunForm.
 */

namespace Drupal\circuit_simulation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;

class CircuitSimulationRunForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'circuit_simulation_run_form';
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $options_first = _list_of_circuit_simulation();
    $url_circuit_simulation_id = (int) arg(2);
    $circuit_simulation_data = _circuit_simulation_information($url_circuit_simulation_id);
    if ($circuit_simulation_data == 'Not found') {
      $url_circuit_simulation_id = '';
    } //$circuit_simulation_data == 'Not found'
    if (!$url_circuit_simulation_id) {
      $selected = !$form_state->getValue(['circuit_simulation']) ? $form_state->getValue(['circuit_simulation']) : key($options_first);
    } //!$url_circuit_simulation_id
    elseif ($url_circuit_simulation_id == '') {
      $selected = 0;
    } //$url_circuit_simulation_id == ''
    else {
      $selected = $url_circuit_simulation_id;
    }
    $form = [];
    $form['circuit_simulation'] = [
      '#type' => 'select',
      '#title' => t('Title of the Circuit Simulation'),
      '#options' => _list_of_circuit_simulation(),
      '#default_value' => $selected,
      '#ajax' => [
        'callback' => 'circuit_simulation_project_details_callback'
        ],
    ];
    if (!$url_circuit_simulation_id) {
      $form['circuit_simulation_details'] = [
        '#type' => 'item',
        '#markup' => '<div id="ajax_circuit_simulation_details"></div>',
      ];
      $form['selected_circuit_simulation'] = [
        '#type' => 'item',
        '#markup' => '<div id="ajax_selected_circuit_simulation"></div>',
      ];
    } //!$url_circuit_simulation_id
    else {
      $circuit_simulation_default_value = $url_circuit_simulation_id;
      $form['circuit_simulation_details'] = [
        '#type' => 'item',
        '#markup' => '<div id="ajax_circuit_simulation_details">' . _circuit_simulation_details($circuit_simulation_default_value) . '</div>',
      ];
      // @FIXME
      // l() expects a Url object, created from a route name or external URI.
      // $form['selected_circuit_simulation'] = array(
      // 			'#type' => 'item',
      // 			'#markup' => '<div id="ajax_selected_circuit_simulation">' . l('Download Abstract', "circuit-simulation-project/download/project-file/" . $circuit_simulation_default_value) . '<br>' . l('Download Circuit Simulation', 'circuit-simulation-project/full-download/project/' . $circuit_simulation_default_value) . '</div>'
      // 		);

    }
    return $form;
  }
public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
}
}
?>
