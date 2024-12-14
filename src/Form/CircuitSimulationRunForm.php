<?php

/**
 * @file
 * Contains \Drupal\circuit_simulation\Form\CircuitSimulationRunForm.
 */

namespace Drupal\circuit_simulation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;
use Drupal\Core\Database\Database;
use Drupal\Core\Link;
use Drupal\Core\Url;
use Symfony\Component\DependencyInjection\ContainerInterface;


class CircuitSimulationRunForm extends FormBase {

 //protected $database;

  /*public function __construct(Connection $database) {
    \Drupal::database() = $database;
  }

  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('database')
    );
  }*/
  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'circuit_simulation_run_form';
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $options_first = $this->_list_of_circuit_simulation();
    //$url_circuit_simulation_id = (int) arg(2);
    $url_circuit_simulation_id = \Drupal::routeMatch()->getParameter('proposal_id');
    $circuit_simulation_data = $this->_circuit_simulation_information($url_circuit_simulation_id);
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
      '#options' => $this->_list_of_circuit_simulation(),
      '#default_value' => $selected,
      '#ajax' => [
        'callback' => '::ajaxProjectDetailsCallback',
        'wrapper' => 'ajax_selected_circuit_simulation'
        ],
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
    $circuit_simulation_default_value = $form_state->getValue('circuit_simulation') ?: $selected;
    $form['update_circuit_simulation']['cs_details'] = [
      '#type' => 'markup',
      '#markup' => $this->_circuit_simulation_details($circuit_simulation_default_value),
      '#states' => [
        'invisible' => [
          ':input[name="circuit_simulation_project"]' => [
            'value' => 0
            ]
          ]
        ],
    ];
   

    $form['update_circuit_simulation']['download_abstract'] = [
      '#type' => 'item',
      '#markup' => Link::fromTextAndUrl('Download Abstract', Url::fromUri('internal:/circuit-simulation-project/download/project-file/' . $circuit_simulation_default_value))->toString() .
              '<br>' .
              Link::fromTextAndUrl('Download Circuit Simulation', Url::fromUri('internal:/circuit-simulation-project/full-download/project/' . $circuit_simulation_default_value))->toString(),
    ];
    /*if (!$url_circuit_simulation_id) {
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
      $form['selected_circuit_simulation'] = array(
      			'#type' => 'item',
      			'#markup' => '<div id="ajax_selected_circuit_simulation">' . l('Download Abstract', "circuit-simulation-project/download/project-file/" . $circuit_simulation_default_value) . '<br>' . l('Download Circuit Simulation', 'circuit-simulation-project/full-download/project/' . $circuit_simulation_default_value) . '</div>'
      		);

    }*/
    return $form;
  }
protected function _list_of_circuit_simulation() {
    $options = ['0' => $this->t('Please select...')];
    $query = \Drupal::database()->select('esim_circuit_simulation_proposal', 'f')
      ->fields('f', ['id', 'project_title', 'name_title', 'contributor_name'])
      ->condition('approval_status', 3)
      ->orderBy('project_title', 'ASC')
      ->execute();

    foreach ($query as $record) {
      $options[$record->id] = "{$record->project_title} (Proposed by {$record->name_title} {$record->contributor_name})";
    }
    
    return $options;
  }
  function _circuit_simulation_information($proposal_id) {
  // Query the database.
  $query = \Drupal::database()->select('esim_circuit_simulation_proposal', 'e')
    ->fields('e')
    ->condition('id', $proposal_id)
    ->condition('approval_status', 3);
  $result = $query->execute()->fetchObject();

  // Return the data or 'Not found'.
  return $result ?: 'Not found';
}

/**
 * Retrieves and formats circuit simulation details.
 */
function _circuit_simulation_details($circuit_simulation_default_value) {
  $circuit_simulation_details = $this->_circuit_simulation_information($circuit_simulation_default_value);

  // Handle the case where no valid circuit simulation is found.
  if ($circuit_simulation_default_value != 0 && $circuit_simulation_details) {
    // Process the reference link if available.
    $reference = !empty($circuit_simulation_details->reference)
      ? preg_replace(
          '~(?:(https?)://([^\s<]+)|(www\.[^\s<]+?\.[^\s<]+))(?<![\.,:])~i',
          '<a href="$0" target="_blank" title="$0">$0</a>',
          $circuit_simulation_details->reference
        )
      : 'Not provided';

    // Generate the title link using Url and Link.
    $title_url = Url::fromUri('internal:/circuit-simulation-project/full-download/project/' . $circuit_simulation_default_value);
    $title_link = Link::fromTextAndUrl($circuit_simulation_details->project_title, $title_url)->toString();

    // Build the markup.
    $markup = '<span style="color: rgb(128, 0, 0);"><strong>About the Circuit Simulation</strong></span><br /><ul>';
    $markup .= '<li><strong>Contributor Name:</strong> ' . $circuit_simulation_details->name_title . ' ' . $circuit_simulation_details->contributor_name . '</li>';
    $markup .= '<li><strong>Title of the Circuit Simulation:</strong> ' . $title_link . '</li>';
    $markup .= '<li><strong>University:</strong> ' . $circuit_simulation_details->university . '</li>';
    if (!empty($circuit_simulation_details->project_guide_name)) {
      $markup .= '<li><strong>Project Guide Name:</strong> ' . $circuit_simulation_details->project_guide_name . '</li>';
    }
    $markup .= '<li><strong>Reference:</strong> ' . $reference . '</li>';
    $markup .= '</ul>';

    return $markup;
  }

  return 'No details available.';
}
public function ajaxProjectDetailsCallback(array &$form, FormStateInterface $form_state) {
return $form['update_circuit_simulation'];
}
public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
}
}
?>
