<?php
// eSim Circuit Simulation display completed proposals
function circuit_simulation_completed_proposals_all()
{
	$output = "";
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('approval_status', 3);
	$query->orderBy('actual_completion_date', 'DESC');
	//$query->condition('is_completed', 1);
	$result = $query->execute();
	
	//var_dump($esim_project_abstract);die;
	if ($result->rowCount() == 0)
	{
		$output .= "Work has been completed for the following circuit simulation. We welcome your contributions." ."<hr>";

	} //$result->rowCount() == 0
	else
	{
		$output .= "Work has been completed for the following circuit simulation. We welcome your contributions." . "<hr>";
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
			$approval_date = date("Y", $row->approval_date);
			$preference_rows[] = array(
				$i,
				l($row->project_title, "circuit-simulation-project/esim-circuit-simulation-run/" . $row->id),
				l('Download', "circuit-simulation-project/download/project-file/" . $row->id),
				$row->contributor_name,
				$row->university,
				$approval_date
			);
			$i++;
		} //$row = $result->fetchObject()
		$preference_header = array(
			'No',
			'Circuit Simulation Project',
			'Abstract File',
			'Contributor Name',
			'Institute',
			'Year'
		);
		$output .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $output;
}
// eSim Circuit Simulation display in progress proposals
function circuit_simulation_progress_all()
{
	$page_content = "";
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('approval_status', 1);
	$query->condition('is_completed', 0);
	$result = $query->execute();
	if ($result->rowCount() == 0)
	{
		$page_content .= "Work is in progress for the following circuit simulation under Circuit Simulation Project<hr>";
	} //$result->rowCount() == 0
	else
	{
		$page_content .= "Work is in progress for the following circuit simulation under Circuit Simulation Project<hr>";
		$preference_rows = array();
		$i = 1;
		while ($row = $result->fetchObject())
		{
			$approval_date = date("Y", $row->approval_date);
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
			'Year'
		);
		$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $page_content;
}