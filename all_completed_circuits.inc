<?php
function all_completed_circuit_simulations($form, $form_state)
{
	$circuit_simulation_activity = array(
		0 => 'All activities',
		1 => 'Circuit Simulation Project',
		2 => 'Circuit Design and Simulation Marathon using eSim',
		3 => 'Mixed Signal Circuit Design and Simulation Marathon',
		4 => 'Mixed Signal SoC design Marathon using eSim & SKY130',
	);
	$selected = isset($form_state['values']['howmany_select']) ? $form_state['values']['howmany_select'] : key($circuit_simulation_activity);
	$form = array();
	$form['howmany_select'] = array(
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
    '#ajax' => array(
      'callback' => 'ajax_selected_activity_callback',
    ),
    //'#suffix' => '<div id="ajax-selected-activity"></div>'

	  );
	    $form['circuit_details_table'] = array(
        '#type' => 'item',
        '#prefix' => '<div id="ajax-selected-activity">',
        '#suffix' => '</div>',
        '#markup' => all_circuit_details($selected)
    );
	return $form;
}


function ajax_selected_activity_callback($form, $form_state)
{
	$commands = array();
	$activity_default_value = $form_state['values']['howmany_select'];
	/*if ($activity_default_value != 0)
	{*/
		$form['howmany_select']['#options'] =  array('All activities' => 'All activities');
		$commands[] = ajax_command_html('#ajax-selected-activity', all_circuit_details($activity_default_value));
		//$commands[] = ajax_command_replace('#ajax_selected_circuit_simulation_action', drupal_render($form['howmany_select']));
		
	//} 
	/*else
	{
		$form['howmany_select']['#options'] =  array('All activities' => 'All activities');
		$commands[] = ajax_command_data('#ajax_selected_circuit_simulation', 'form_state_value_select', $form_state['values']['howmany_select']);
	}*/
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}

function all_circuit_details($activity_id){
	if($activity_id == 0){
	$page_content = "";
	$result = db_query("select * from (select concat('https://esim.fossee.in/circuit-simulation-project/esim-circuit-simulation-run/', e.id) as link,from_unixtime(e.actual_completion_date, '%Y') as actual_completion_date, e.project_title as circuit_title, e.contributor_name as contributor_name, e.university as institute, 'Circuit Simulation' as activity
from esim_circuit_simulation_proposal as e where e.approval_status = 3
union all
select concat('https://esim.fossee.in/hackathon-submission/download/completed-circuit/',h1.id) as link, '2021' as actual_completion_date,  h1.circuit_name as circuit_title, h1.participant_name as contributor_name, h1.institute  as institute, 'Circuit Design and Simulation Marathon using eSim' as activity from hackathon_completed_circuits as hcc
left join hackathon_literature_survey as h1
on hcc.literature_survey_id = h1.id
union all
select concat('https://esim.fossee.in/mixed-signal-design-marathon/download/circuits/',h2.id) as link, '2022(Feb.)' as actual_completion_date, h2.circuit_name as circuit_title, h2.participant_name as contributor_name, h2.institute as institute, 'Mixed Signal Circuit Design and Simulation Marathon' as activity from mixed_signal_marathon_final_submission as msm
left join mixed_signal_marathon_literature_survey as h2
on msm.literature_survey_id = h2.id where msm.approval_status = 3
union all
select concat('https://esim.fossee.in/mixed-signal-soc-design-marathon/download/circuits/', h3.id) as link, '2022(Sep.)' as actual_completion_date, h3.circuit_name as circuit_title, h3.participant_name as contributor_name, h3.institute as institute, 'Mixed Signal SoC design Marathon using eSim & SKY130' as activity from mixed_signal_soc_marathon_final_submission as soc
left join mixed_signal_soc_marathon_literature_survey as h3
on soc.literature_survey_id = h3.id where soc.approval_status = 3) as dum
order by dum.actual_completion_date desc");
	$i = $result->rowCount();
	while($row = $result->fetchObject()){
		$preference_rows[] = array(
			$i,
			l($row->circuit_title, $row->link),
			$row->contributor_name,
			$row->institute,
			$row->activity,
			$row->actual_completion_date
		);
	$i--;
	}
	$preference_header = array(
			'No',
			'Name of the Circuit', 
			'Contributor Name',
			'University / Institute',
			'Activity name',
			'Year'
		);
	$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));

		}
	else if($activity_id == 1){
		$result = db_query("select e.id, e.project_title as circuit_title, e.contributor_name as contributor_name, e.university as institute, from_unixtime(actual_completion_date, '%Y ') as year
from esim_circuit_simulation_proposal as e where e.approval_status = 3 order by actual_completion_date desc");
	$i = $result->rowCount();
	while($row = $result->fetchObject()){
		$preference_rows[] = array(
			$i,
			l($row->circuit_title, 'https://esim.fossee.in/circuit-simulation-project/esim-circuit-simulation-run/' . $row->id),
			$row->contributor_name,
			$row->institute,
			$row->year
		);
	$i--;
	}
	$preference_header = array(
			'No',
			'Name of the Circuit',
			'Contributor Name',
			'University / Institute',
			'Year'
		);
	$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
		
	}
	else if($activity_id  == 2){
		$result = db_query("select h1.id, '2021' as actual_completion_date,  h1.circuit_name as circuit_title, h1.participant_name as contributor_name, h1.institute  as institute from hackathon_completed_circuits as hcc
left join hackathon_literature_survey as h1 on hcc.literature_survey_id = h1.id");
	$i = $result->rowCount();
	while($row = $result->fetchObject()){
		$preference_rows[] = array(
			$i,
			l($row->circuit_title, 'https://esim.fossee.in/hackathon-submission/download/completed-circuit/' . $row->id),
			$row->contributor_name,
			$row->institute,
			$row->actual_completion_date
		);
	$i--;
	}
	$preference_header = array(
			'No',
			'Name of the Circuit', 
			'Contributor Name',
			'University / Institute',
			'Year'
		);
	$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	else if($activity_id == 3){
		$result = db_query("select h2.id, '2022(Feb.)' as actual_completion_date, h2.circuit_name as circuit_title, h2.participant_name as contributor_name, h2.institute as institute from mixed_signal_marathon_final_submission as msm
left join mixed_signal_marathon_literature_survey as h2 on msm.literature_survey_id = h2.id where msm.approval_status = 3");
	$i = $result->rowCount();
	while($row = $result->fetchObject()){
		$preference_rows[] = array(
			$i,
			l($row->circuit_title, 'https://esim.fossee.in/mixed-signal-design-marathon/download/circuits/' . $row->id),
			$row->contributor_name,
			$row->institute,
			$row->actual_completion_date
		);
	$i--;
	}
	$preference_header = array(
			'No',
			'Name of the Circuit', 
			'Contributor Name',
			'University / Institute',
			'Year'
		);
	$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	else if($activity_id == 4){
		$result = db_query("select h3.id, '2022(Sep.)' as actual_completion_date, h3.circuit_name as circuit_title, h3.participant_name as contributor_name, h3.institute as institute from mixed_signal_soc_marathon_final_submission as soc
left join mixed_signal_soc_marathon_literature_survey as h3
on soc.literature_survey_id = h3.id where soc.approval_status = 3");
	$i = $result->rowCount();
	while($row = $result->fetchObject()){
		$preference_rows[] = array(
			$i,
			l($row->circuit_title, 'https://esim.fossee.in/mixed-signal-soc-design-marathon/download/circuits/' . $row->id),
			$row->contributor_name,
			$row->institute,
			$row->actual_completion_date
		);
	$i--;
	}
	$preference_header = array(
			'No',
			'Name of the Circuit', 
			'Contributor Name',
			'University / Institute',
			'Year'
		);
	$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	return $page_content;
	}

/*
select e.project_title as circuit_title, e.contributor_name as contributor_name, e.university as institute, 'Circuit Simulation' as activity
from esim_circuit_simulation_proposal as e where e.approval_status = 3
union all
select h1.circuit_name as circuit_title, h1.participant_name as contributor_name, h1.institute  as institute, 'Circuit Design and Simulation Marathon using eSim' as activity from hackathon_completed_circuits as hcc
left join hackathon_literature_survey as h1
on hcc.literature_survey_id = h1.id
union all
select h2.circuit_name as circuit_title, h2.participant_name as contributor_name, h2.institute as institute, 'Mixed Signal Circuit Design and Simulation Marathon' as activity from mixed_signal_marathon_final_submission as msm
left join mixed_signal_marathon_literature_survey as h2
on msm.literature_survey_id = h2.id where msm.approval_status = 3
union all
select h3.circuit_name as circuit_title, h3.participant_name as contributor_name, h3.institute as institute, 'Mixed Signal SoC design Marathon using eSim & SKY130' as activity from mixed_signal_soc_marathon_final_submission as soc
left join mixed_signal_soc_marathon_literature_survey as h3
on soc.literature_survey_id = h3.id where soc.approval_status = 3*/