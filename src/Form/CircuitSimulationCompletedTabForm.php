<?php

/**
 * @file
 * Contains \Drupal\circuit_simulation\Form\CircuitSimulationCompletedTabForm.
 */

namespace Drupal\circuit_simulation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;
use Drupal\Core\Database\Database;
use Drupal\Core\Render\Markup;
use Drupal\Core\Ajax\AjaxResponse;
use Drupal\Core\Ajax\HtmlCommand;
use Drupal\Core\Ajax\ReplaceCommand;
use Drupal\Core\Link;
use Drupal\Core\Url;


class CircuitSimulationCompletedTabForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'circuit_simulation_completed_tab_form';
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $options_first = $this->_circuit_simulation_details_year_wise();
    $selected = !$form_state->getValue(['howmany_select']) ? $form_state->getValue(['howmany_select']) : key($options_first);
    $form = [];
    $form['howmany_select'] = [
      '#title' => t('Sorting projects according to year:'),
      '#type' => 'select',
      '#options' => $this->_circuit_simulation_details_year_wise(),
      /*'#options' => array(
    	'Please select...' => 'Please select...',
    	'2017' => '2017',
    	'2018' => '2018', 
    	'2019' => '2019', 
    	'2020' => '2020', 
    	'2021' => '2021'),*/
      //'#default_value' => $selected,
      '#ajax' => [
        'callback' => '::ajax_completed_projects_year',
        'wrapper' => 'ajax_selected_year'
        ],
      //'#suffix' => '<div id="ajax_selected_year"></div>',
    ];
    $form['update_activity_table'] = [
      '#type' => 'container',
      '#attributes' => ['id'=> 'ajax_selected_year']
    ];
    $activity_default_value = $form_state->getValue('howmany_select');
    // $form['update_activity_table']['year'] = [
    //   '#type' => 'item',
    //   '#markup' => Markup::create('hi' . $activity_default_value)
    // ];
    //var_dump($this->_circuit_simulation_details('2023'));die;
    $table = $this->_circuit_simulation_details($activity_default_value);
    //var_dump($activity_default_value);die;
    $form['update_activity_table']['circuit_details_table'] = [
      '#type' => 'fieldset',
      //'#markup' => $this->all_circuit_details($activity_default_value),
    ];
    $form['update_activity_table']['circuit_details_table']['table'] = $table;
    return $form;
  }
  public function ajax_completed_projects_year(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
  return $form['update_activity_table'];
}
  function _circuit_simulation_details($circuit_simulation_proposal_id) {
  // Initialize output string.

  $markup = "";
//var_dump($circuit_simulation_proposal_id);die;
  // Get the database connection.
  $connection = Database::getConnection();
    $sql = "SELECT *
          FROM esim_circuit_simulation_proposal
          WHERE approval_status = :status AND FROM_UNIXTIME(actual_completion_date, '%Y') = :year
          ORDER BY actual_completion_date DESC";

  // Execute the query, passing parameters for security (binding the status).
  $result = $connection->query($sql, [':status' => 3, 
    ':year' => $circuit_simulation_proposal_id]);
$rows = $result->fetchAll();
$i = count($rows);

  // Check if any rows are returned.
  if ($i == 0) {
    $markup .= "No proposals found for the selected year: " . $i . "<hr>";
  }
  else {
    $markup .= "Work has been completed for the following circuit simulation: " . $i . "<hr>";
    foreach ($rows as $row) {
      $circuit_title = is_string($row->project_title) ? $row->project_title : implode(', ', (array) $row->project_title);
         //var_dump($circuit_title);die;
  //$link = is_string($row->link) ? $row->link : reset((array) $row->link);
  // Build the link
  $url = Url::fromUri("internal:/circuit-simulation-project/esim-circuit-simulation-run/" . $row->id);
  $link_render = Link::fromTextAndUrl($circuit_title, $url)->toString();
      $preference_rows[] = [
        $i,
       $link_render,
        $row->contributor_name,
        $row->university,
        date("Y", $row->actual_completion_date)
      ];
      //var_dump($result);die;
      $i--;
    }

    // Define the table header.
    $preference_header = array(
      'No',
      'Circuit Simulation Project',
      'Contributor Name',
      'University / Institute',
      'Year of Completion'
    );

    // Render the table using Drupal 10 theming.
    $output = [
      '#type' => 'table',
      '#header' => $preference_header,
      '#rows' => $preference_rows
    ];
  }

  // Return the output.
  return [
      'markup' => [
        '#markup' => $markup,
      ],
      'table' => $output,
    ];
}
  function _circuit_simulation_details_year_wise() {
  // Initialize the years array with the default option.
  $circuit_simulation_years = array(
    '0' => 'Please select...'
  );

  // Get the database connection.
  $connection = Database::getConnection();

  // Write the query to select the distinct years.
  $sql = "SELECT DISTINCT FROM_UNIXTIME(actual_completion_date, '%Y') AS Year
          FROM esim_circuit_simulation_proposal
          WHERE approval_status = :status
          ORDER BY Year ASC";

  // Execute the query, passing parameters for security (binding the status).
  $result = $connection->query($sql, [':status' => 3]);
  foreach ($result as $year_wise_list_data) {
    $circuit_simulation_years[$year_wise_list_data->Year] = $year_wise_list_data->Year;
  }

  // Return the array of years.
  return $circuit_simulation_years;
}
public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
}
}
?>
