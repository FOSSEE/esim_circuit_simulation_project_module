<?php
// eSim Circuit Simulation display completed proposals
function circuit_simulation_completed_proposals_all()
{
	$output = "";
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('approval_status', 3);
	//$query->condition('is_completed', 1);
	$result = $query->execute();
	if ($result->rowCount() == 0)
	{
		$output .= "Work has been completed for the following circuit simulation. We welcome your contributions. For more details, please visit ".l("http://esim.fossee.in/circuit-simulation-project","http://esim.fossee.in/circuit-simulation-project")."<br>"."<h4>"."If you are looking for circuit-simulation project ideas, ".l("click here","http://esim.fossee.in/circuit-simulation-ideas",array('attributes' => array('class' => array('flash_content'))))."</h4>"."<hr>";

	} //$result->rowCount() == 0
	else
	{
		$output .= "Work has been completed for the following circuit simulation. We welcome your contributions. For more details, please visit ".l("http://esim.fossee.in/circuit-simulation-project","http://esim.fossee.in/circuit-simulation-project")."<br>"."<h4>"."If you are looking for circuit-simulation project ideas, ".l("click here","http://esim.fossee.in/circuit-simulation-ideas",array('attributes' => array('class' => array('flash_content'))))."</h4>"."<hr>";
		$preference_rows = array();
		$i = 1;
		while ($row = $result->fetchObject())
		{
			$approval_date = date("Y", $row->approval_date);
			$preference_rows[] = array(
				$i,
				l($row->project_title, "circuit-simulation-project/esim-circuit-simulation-run/" . $row->id, array('attributes' => array('title' => 'This is a zip file containing a pdf (abstract) and a dwxml/dwxmz file which is the eSim circuit simulation which is to be viewed by right clicking on the file and opening with eSim.'))),
				$row->university,
				$approval_date
			);
			$i++;
		} //$row = $result->fetchObject()
		$preference_header = array(
			'No',
			'Circuit Simulation Project',
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
		$page_content .= "<ol>";
		while ($row = $result->fetchObject())
		{
			$page_content .= "<li>";
			$page_content .= $row->project_title;
			$page_content .= "</li>";
		} //$row = $result->fetchObject()
		$page_content .= "</ol>";
	}
	return $page_content;
}
