<?php

/**
 * @file
 * Contains \Drupal\circuit_simulation\Form\AllCompletedCircuitSimulations.
 */

namespace Drupal\circuit_simulation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;
use Drupal\Core\Database\Database;
use Drupal\Core\Link;
use Drupal\Core\Url;
use Drupal\Core\Render\Markup;
use Drupal\Core\Ajax\AjaxResponse;
use Drupal\Core\Ajax\HtmlCommand;
use Drupal\Core\Ajax\ReplaceCommand;
use Drupal\Core\Routing\TrustedRedirectResponse;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\RedirectResponse;

class AllCompletedCircuitSimulations extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'all_completed_circuit_simulations';
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $circuit_simulation_activity = $this->_list_of_all_completed_circuits();
    //var_dump($circuit_simulation_activity);die;
    //$activity_short_name = arg(1);
    $activity_short_name = \Drupal::routeMatch()->getParameter('activity_name');
    //var_dump($activity_short_name);die;
    $form = [];
    if (!$activity_short_name) {
      $selected = !$form_state->getValue(['howmany_select']) ? $form_state->getValue(['howmany_select']) : key($circuit_simulation_activity);
      //var_dump($selected);die;
    } //!$url_circuit_simulation_id
    else {
      $selected = $activity_short_name;
    }
    /*if($activity_short_name == NULL){
      $selected = 'all';
    }*/
    //var_dump(key($circuit_simulation_activity));die;
    //$selected = !$activity_short_name ? $form_state->getValue(['howmany_select']) : $activity_short_name;    
    //var_dump($selected);die;
    $form['howmany_select'] = [
      '#title' => t('Display completed circuits from'),
      '#type' => 'select',
      '#options' => $circuit_simulation_activity,
      '#default_value' => $selected,
      '#ajax' => [
        'callback' => '::ajax_selected_activity_callback',
        'wrapper' => 'ajax_selected_activity'
        ],
      //'#suffix' => '<div id="ajax-selected-activity"></div>'

    ];
    $form['update_activity_table'] = [
      '#type' => 'container',
      '#attributes' => ['id' => 'ajax_selected_activity']
    ];
    //var_dump($selected);die;
    $activity_default_value = $form_state->getValue('howmany_select') ?: $selected;
    
    $table = $this->all_circuit_details($activity_default_value);
    //var_dump($activity_default_value);die;
    $form['update_activity_table']['circuit_details_table'] = [
      '#type' => 'fieldset',
      //'#markup' => $this->all_circuit_details($activity_default_value),
    ];
    $form['update_activity_table']['circuit_details_table']['table'] = $table;
    return $form;
  }
public function ajax_selected_activity_callback(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
  return $form['update_activity_table'];
}
function _list_of_all_completed_circuits() {
  $circuit_simulation_titles = [
    'all' => 'All activities',
  ];

  // Use the Database API to query the table.
  $connection = Database::getConnection();
  $query = $connection->select('all_completed_activities', 'a');
  $query->fields('a', ['activity_short_name', 'activity_name']);
  $result = $query->execute();

  foreach ($result as $record) {
    $circuit_simulation_titles[$record->activity_short_name] = $record->activity_name;
  }

  return $circuit_simulation_titles;
}
  public function all_circuit_details($activity_id) {
    $connection = Database::getConnection();
    //var_dump($activity_id);die;
  switch ($activity_id) {
    case 'all':
    //var_dump($activity_id);die;
      $query = Database::getConnection()->query("SELECT * FROM (
          SELECT 
            CONCAT('https://esim.fossee.in/circuit-simulation-project/esim-circuit-simulation-run/', e.id) AS link,
            FROM_UNIXTIME(e.actual_completion_date, '%Y') AS actual_completion_date,
            e.project_title AS circuit_title,
            e.contributor_name AS contributor_name,
            e.university AS institute,
            'Circuit Simulation' AS activity
          FROM esim_circuit_simulation_proposal e 
          WHERE e.approval_status = 3
          UNION ALL
          SELECT 
            CONCAT('https://esim.fossee.in/hackathon/download/completed-circuits/', h1.id) AS link,
            '2021' AS actual_completion_date,
            h1.circuit_name AS circuit_title,
            h1.participant_name AS contributor_name,
            h1.institute AS institute,
            'Circuit Design and Simulation Marathon using eSim' AS activity
          FROM hackathon_completed_circuits hcc
          LEFT JOIN hackathon_literature_survey h1 ON hcc.literature_survey_id = h1.id
          UNION ALL
          SELECT 
            CONCAT('https://esim.fossee.in/mixed-signal-design-marathon/download/circuits/', h2.id) AS link,
            '2022(Feb.)' AS actual_completion_date,
            h2.circuit_name AS circuit_title,
            h2.participant_name AS contributor_name,
            h2.institute AS institute,
            'Mixed Signal Circuit Design and Simulation Marathon' AS activity
          FROM mixed_signal_marathon_final_submission msm
          LEFT JOIN mixed_signal_marathon_literature_survey h2 ON msm.literature_survey_id = h2.id
          WHERE msm.approval_status = 3
          UNION ALL
          SELECT 
            CONCAT('https://esim.fossee.in/mixed-signal-soc-design-marathon/download/circuits/', h3.id) AS link,
            '2022(Sep.)' AS actual_completion_date,
            h3.circuit_name AS circuit_title,
            h3.participant_name AS contributor_name,
            h3.institute AS institute,
            'Mixed Signal SoC Design Marathon using eSim & SKY130' AS activity
          FROM mixed_signal_soc_marathon_final_submission soc
          LEFT JOIN mixed_signal_soc_marathon_literature_survey h3 ON soc.literature_survey_id = h3.id
          WHERE soc.approval_status = 3
        ) AS dum
        ORDER BY dum.actual_completion_date DESC");
      /*$count_query = $query->countQuery();
      $i = $count_query->execute()->fetchField();*/
$rows = $query->fetchAll();
$i = count($rows);
      foreach($rows as $row) {
   // var_dump($row);die;
         $circuit_title = is_string($row->circuit_title) ? $row->circuit_title : implode(', ', (array) $row->circuit_title);
         //var_dump($circuit_title);die;
  $link = is_string($row->link) ? $row->link : reset((array) $row->link);
  // Build the link
  $url = Url::fromUri($link);
  $link_render = Link::fromTextAndUrl($circuit_title, $url)->toString();
//var_dump($link_render);die;
    $preference_rows[] = [
      $i,
      $link_render,
     $row->contributor_name,
      $row->institute,
     $row->activity,
      $row->actual_completion_date
    ];

    $i--;
    //var_dump($preference_rows);die;
  }
//var_dump(count($preference_rows));die;
  // Table header.
  $preference_header = [
    'No',
    'Name of the Circuit',
    'Contributor Name',
    'University / Institute',
    'Activity',
    'Year',
  ];
  //return ['#markup' => Markup::create('<p>All activities</p>')];
      break;

    case 'csp':
      $query = Database::getConnection()->query("SELECT 
            CONCAT('https://esim.fossee.in/circuit-simulation-project/esim-circuit-simulation-run/', e.id) AS link,
            FROM_UNIXTIME(e.actual_completion_date, '%Y') AS actual_completion_date,
            e.project_title AS circuit_title,
            e.contributor_name AS contributor_name,
            e.university AS institute,
            'Circuit Simulation' AS activity
          FROM esim_circuit_simulation_proposal e 
          WHERE e.approval_status = 3 ORDER BY e.actual_completion_date DESC");
      $rows = $query->fetchAll();
$i = count($rows);
      foreach($rows as $row) {
    //var_dump($row->activity);die;
    $preference_rows[] = [
      $i,
      Link::fromTextAndUrl($row->circuit_title, Url::fromUri($row->link))->toString(),
      $row->contributor_name,
      $row->institute,
      //$row->activity,
      $row->actual_completion_date,
    ];
    $i--;
  }

  // Table header.
  $preference_header = [
    'No',
    'Name of the Circuit',
    'Contributor Name',
    'University / Institute',
    //'Activity',
    'Year',
  ];
  //return $
      break;

    case 'cdsm':
      $query = Database::getConnection()->query("
        SELECT 
          h1.id, 
          CONCAT('https://esim.fossee.in/hackathon/download/completed-circuits/', h1.id) AS link,
          '2021' AS actual_completion_date,
          h1.circuit_name AS circuit_title,
          h1.participant_name AS contributor_name,
          h1.institute AS institute
        FROM hackathon_completed_circuits hcc
        LEFT JOIN hackathon_literature_survey h1 ON hcc.literature_survey_id = h1.id
      ");
      $rows = $query->fetchAll();
$i = count($rows);
      foreach($rows as $row) {
   // var_dump($row);die;
    $preference_rows[] = [
      $i,
      Link::fromTextAndUrl($row->circuit_title, Url::fromUri($row->link))->toString(),
      $row->contributor_name,
      $row->institute,
      //$row->activity,
      $row->actual_completion_date,
    ];
    $i--;
  }

  // Table header.
  $preference_header = [
    'No',
    'Name of the Circuit',
    'Contributor Name',
    'University / Institute',
    //'Activity/Year',
    'Year',
  ];
      break;

    case 'mscd':
      $query = Database::getConnection()->query("
        SELECT 
          h2.id,
          CONCAT('https://esim.fossee.in/mixed-signal-design-marathon/download/circuits/', h2.id) AS link,
          '2022(Feb.)' AS actual_completion_date,
          h2.circuit_name AS circuit_title,
          h2.participant_name AS contributor_name,
          h2.institute AS institute
        FROM mixed_signal_marathon_final_submission msm
        LEFT JOIN mixed_signal_marathon_literature_survey h2 ON msm.literature_survey_id = h2.id
        WHERE msm.approval_status = 3
      ");
     $rows = $query->fetchAll();
$i = count($rows);
      foreach($rows as $row) {
    //var_dump($row->activity);die;
    $preference_rows[] = [
      $i,
      Link::fromTextAndUrl($row->circuit_title, Url::fromUri($row->link))->toString(),
      $row->contributor_name,
      $row->institute,
      //$row->activity,
      $row->actual_completion_date,
    ];
    $i--;
  }

  // Table header.
  $preference_header = [
    'No',
    'Name of the Circuit',
    'Contributor Name',
    'University / Institute',
    //'Activity/Year',
    'Year',
  ];
      break;

    case 'mscd-sky130':
      $query = Database::getConnection()->query("
        SELECT 
          h3.id,
          CONCAT('https://esim.fossee.in/mixed-signal-soc-design-marathon/download/circuits/', h3.id) AS link,
          '2022(Sep.)' AS actual_completion_date,
          h3.circuit_name AS circuit_title,
          h3.participant_name AS contributor_name,
          h3.institute AS institute
        FROM mixed_signal_soc_marathon_final_submission soc
        LEFT JOIN mixed_signal_soc_marathon_literature_survey h3 ON soc.literature_survey_id = h3.id
        WHERE soc.approval_status = 3
      ");
     $rows = $query->fetchAll();
$i = count($rows);
      foreach($rows as $row) {
    //var_dump($row->activity);die;
    $preference_rows[] = [
      $i,
      Link::fromTextAndUrl($row->circuit_title, Url::fromUri($row->link))->toString(),
      $row->contributor_name,
      $row->institute,
      //$row->activity,
      $row->actual_completion_date,
    ];
    $i--;
  }

  // Table header.
  $preference_header = [
    'No',
    'Name of the Circuit',
    'Contributor Name',
    'University / Institute',
    //'Activity/Year',
    'Year',
  ];
      break;

    /*default:
      return ['#markup' => Markup::create('<p>Invalid activity ID provided.</p>')];*/
  }

  // Render table.
  $page_content = [
    '#type' => 'table',
    '#header' => $preference_header,
    '#rows' => $preference_rows,
  ];

  return $page_content;
}
public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
}
}
?>
