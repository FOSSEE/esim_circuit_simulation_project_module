<?php

/**
 * @file
 * Contains \Drupal\circuit_simulation\Form\AllCompletedCircuitSimulations.
 */

namespace Drupal\circuit_simulation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;

class AllCompletedCircuitSimulations extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'all_completed_circuit_simulations';
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $circuit_simulation_activity = _list_of_all_completed_circuits();
    $activity_short_name = arg(1);
    /*switch ($activity_short_name) {
		case 'all':
			$url_circuit_simulation_id = 0;
			break;
		case 'all':
			$url_circuit_simulation_id = 1;
			break;
		
		default:
			// code...
			break;
	}*/
    if (!$activity_short_name) {
      $selected = !$form_state->getValue(['howmany_select']) ? $form_state->getValue(['howmany_select']) : key($circuit_simulation_activity);
    } //!$url_circuit_simulation_id
    elseif ($activity_short_name == '') {
      $selected = 0;
    } //$url_circuit_simulation_id == ''
    else {
      $selected = $activity_short_name;
    }
    //var_dump($activity_short_name);die;
	/*$circuit_simulation_activity = array(
		0 => 'All activities',
		1 => 'Circuit Simulation Project',
		2 => 'Circuit Design and Simulation Marathon using eSim',
		3 => 'Mixed Signal Circuit Design and Simulation Marathon',
		4 => 'Mixed Signal SoC design Marathon using eSim & SKY130',
	);*/
    /*$selected = isset($form_state['values']['howmany_select']) ? $form_state['values']['howmany_select'] : key($circuit_simulation_activity);*/
    //var_dump($selected);die;
    $form = [];
    $form['howmany_select'] = [
      '#title' => t('Display completed circuits from'),
      '#type' => 'select',
      '#options' => $circuit_simulation_activity,
      /*'#options' => array(
    	'Please select...' => 'Please select...',
    	'2017' => '2017',
    	'2018' => '2018', 
    	'2019' => '2019', 
    	'2020' => '2020', 
    	'2021' => '2021'),*/
      '#default_value' => $selected,
      '#ajax' => [
        'callback' => 'ajax_selected_activity_callback'
        ],
      //'#suffix' => '<div id="ajax-selected-activity"></div>'

    ];
    $form['circuit_details_table'] = [
      '#type' => 'item',
      '#prefix' => '<div id="ajax-selected-activity">',
      '#suffix' => '</div>',
      '#markup' => all_circuit_details($selected),
    ];
    return $form;
  }
public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
}
}
?>
