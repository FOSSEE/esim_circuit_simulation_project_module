<?php

//Proposal tab for reviewing approved proposals
function circuit_simulation_approved_tab()
{
	$page_content = "";
	$result = db_query("SELECT * from esim_circuit_simulation_proposal where id not in (select proposal_id from esim_circuit_simulation_submitted_abstracts) AND approval_status = 1 order by approval_date desc");
	if ($result->rowCount() == 0)
	{
		$page_content .= "Work is in progress for the following circuit simulation under Circuit Simulation Project: " . $result->rowCount() . "<hr>";
	} //$result->rowCount() == 0
	else
	{
		$page_content .= "Work is in progress for the following circuit simulation under Circuit Simulation Project: " . $result->rowCount() . "<hr>";
		$preference_rows = array();
		$i = 1;
		while ($row = $result->fetchObject())
		{
			$approval_date = date("d-M-Y", $row->approval_date);
			$preference_rows[] = array(
				$i,
				$row->project_title,
				$row->contributor_name,
				$row->university,
				$approval_date
			);
			$i++;
		} //$row = $result->fetchObject()
		$preference_header = array(
			'No',
			'Circuit Simulation Project',
			'Contributor Name',
			'Institute',
			'Date of Approval'
		);
		$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $page_content;
}



///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

//Proposal tab for reviewing Code submission of the uploaded proposals  

function circuit_simulation_uploaded_tab()
{
	$page_content = "";
	$result = db_query("SELECT dfp.project_title, dfp.contributor_name, dfp.id, dfp.university, dfa.abstract_upload_date, dfa.abstract_approval_status from esim_circuit_simulation_proposal as dfp JOIN esim_circuit_simulation_submitted_abstracts as dfa on dfa.proposal_id = dfp.id where dfp.id in (select proposal_id from esim_circuit_simulation_submitted_abstracts) AND approval_status = 1");

	if ($result->rowCount() == 0)
	{
		$page_content .= "Uploaded Proposals under Circuit Simulation Project<hr>";
	}
	else
	{
		$page_content .= "Uploaded Proposals under Circuit Simulation Project: " . $result->rowCount() . "<hr>";
		$preference_rows = array();
		$i = 1;
		while ($row = $result->fetchObject())
		{
			$abstract_upload_date = date("d-M-Y", $row->abstract_upload_date);
			$preference_rows[] = array(
				$i,
				$row->project_title,
				$row->contributor_name,
				$row->university,
				$abstract_upload_date
			);
			$i++;
		}
		$preference_header = array(
			'No',
			'Circuit Simulation Project',
			'Contributor Name',
			'University / Institute',
			'Date of file submission'
		);
		$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $page_content;
}


///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

//Proposal tab for reviewing Completed proposals

function circuit_simulation_completed_tab_form($form, $form_state)
{
	$options_first = _circuit_simulation_details_year_wise();
	$selected = isset($form_state['values']['howmany_select']) ? $form_state['values']['howmany_select'] : key($options_first);
	$form = array();
	$form['howmany_select'] = array(
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
    '#ajax' => array(
      'callback' => 'ajax_example_autocheckboxes_callback',
    ),
    '#suffix' => '<div id="ajax-selected-circuit_simulation"></div>'

	  );
	return $form;
}


function ajax_example_autocheckboxes_callback($form, $form_state)
{
	$commands = array();
	$circuit_simulation_project_default_value = $form_state['values']['howmany_select'];
	if ($circuit_simulation_project_default_value != 0)
	{
		$form['howmany_select']['#options'] =  array('Please select...' => 'Please select...');
		$commands[] = ajax_command_html('#ajax-selected-circuit_simulation', _circuit_simulation_details($circuit_simulation_project_default_value));
		$commands[] = ajax_command_replace('#ajax_selected_circuit_simulation_action', drupal_render($form['howmany_select']));
		
	} 
	else
	{
		$form['howmany_select']['#options'] =  array('Please select...' => 'Please select...');
		$commands[] = ajax_command_data('#ajax_selected_circuit_simulation', 'form_state_value_select', $form_state['values']['howmany_select']);
	}
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}


function _circuit_simulation_details_year_wise()
  {
    $circuit_simulation_years = array(
		'0' => 'Please select...'
	);
    $result = db_query("SELECT from_unixtime(actual_completion_date, '%Y ') as Year from esim_circuit_simulation_proposal WHERE approval_status = 3 ORDER BY Year ASC");

    while ($year_wise_list_data = $result->fetchObject())
      {
        $circuit_simulation_years[$year_wise_list_data->Year] = $year_wise_list_data->Year;
      }
    return $circuit_simulation_years;
  }


function _circuit_simulation_details($circuit_simulation_proposal_id)
{
	$output = "";
	$result = db_query("SELECT * from {esim_circuit_simulation_proposal} WHERE approval_status = 3 and from_unixtime(actual_completion_date, '%Y') = :year",
		array(
							':year' => $circuit_simulation_proposal_id)
				 );
	
	if ($result->rowCount() == 0)
	{
		$output .= "Work has been completed for the following circuit simulation: " . $result->rowCount() . "<hr>";

	} //$result->rowCount() == 0
	else
	{
		$output .= "Work has been completed for the following circuit simulation: " . $result->rowCount() . "<hr>";
		$preference_rows = array();
		$i = 1;
		while ($row = $result->fetchObject())
		{
			$proposal_id = $row->id;
			$query1 = db_select('esim_circuit_simulation_submitted_abstracts_file');
			$query1->fields('esim_circuit_simulation_submitted_abstracts_file');
			$query1->condition('file_approval_status', 1);
			$query1->condition('proposal_id', $proposal_id);
			$esim_project_files = $query1->execute();
			$esim_project_abstract = $esim_project_files->fetchObject();
			$completion_date = date("Y", $row->actual_completion_date);
			$preference_rows[] = array(
				$i,
				l($row->project_title, "circuit-simulation-project/esim-circuit-simulation-run/" . $row->id),
				$row->contributor_name,
				$row->university,
				$completion_date
			);
			$i++;
		} //$row = $result->fetchObject()
		$preference_header = array(
			'No',
			'Circuit Simulation Project',
			'Contributor Name',
			'University / Institute',
			'Year of Completion'
		);
		$output .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $output;
}