<?php
function circuit_simulation_run_form($form, &$form_state)
{
	$options_first = _list_of_circuit_simulation();
	$url_circuit_simulation_id = (int) arg(2);
	$circuit_simulation_data = _circuit_simulation_information($url_circuit_simulation_id);
	if ($circuit_simulation_data == 'Not found') {
		$url_circuit_simulation_id = '';
	} //$circuit_simulation_data == 'Not found'
	if (!$url_circuit_simulation_id) {
		$selected = isset($form_state['values']['circuit_simulation']) ? $form_state['values']['circuit_simulation'] : key($options_first);
	} //!$url_circuit_simulation_id
	elseif ($url_circuit_simulation_id == '') {
		$selected = 0;
	} //$url_circuit_simulation_id == ''
	else {
		$selected = $url_circuit_simulation_id;
	}
	$form = array();
	$form['circuit_simulation'] = array(
		'#type' => 'select',
		'#title' => t('Title of the Circuit Simulation'),
		'#options' => _list_of_circuit_simulation(),
		'#default_value' => $selected,
		'#ajax' => array(
			'callback' => 'circuit_simulation_project_details_callback'
		)
	);
	if (!$url_circuit_simulation_id) {
		$form['circuit_simulation_details'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_circuit_simulation_details"></div>'
		);
		$form['selected_circuit_simulation'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_selected_circuit_simulation"></div>'
		);
	} //!$url_circuit_simulation_id
	else {
		$circuit_simulation_default_value = $url_circuit_simulation_id;
		$form['circuit_simulation_details'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_circuit_simulation_details">' . _circuit_simulation_details($circuit_simulation_default_value) . '</div>'
		);
		$form['selected_circuit_simulation'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_selected_circuit_simulation">' . l('Download Circuit Simulation', 'circuit-simulation-project/full-download/project/' . $circuit_simulation_default_value) . '</div>'
		);
	}
	return $form;
}
function circuit_simulation_project_details_callback($form, $form_state)
{
	$commands = array();
	$circuit_simulation_default_value = $form_state['values']['circuit_simulation'];
	if ($circuit_simulation_default_value != 0) {
		$form['circuit_simulation_details']['#markup'] = _circuit_simulation_details($circuit_simulation_default_value);
		$circuit_simulation_details = _circuit_simulation_information($circuit_simulation_default_value);
		$provider = user_load($circuit_simulation_details->uid);
		if ($circuit_simulation_details->uid > 0) {
			$commands[] = ajax_command_html('#ajax_selected_circuit_simulation', l('Download Circuit Simulation', 'circuit-simulation-project/full-download/project/' . $circuit_simulation_default_value));
		} //$circuit_simulation_details->uid > 0
		else {
			$commands[] = ajax_command_html('#ajax_selected_circuit_simulation', '');
			$commands[] = ajax_command_html('#ajax_selected_circuit_simulation_esim', '');
		}
		$commands[] = ajax_command_html('#ajax_circuit_simulation_details', _circuit_simulation_details($circuit_simulation_default_value));
	} //$circuit_simulation_default_value != 0
	else {
		// $form['lab_experiment_list']['#options'] = _ajax_get_experiment_list();
		//  $commands[] = ajax_command_replace('#ajax_selected_experiment', drupal_render($form['lab_experiment_list']));
		$commands[] = ajax_command_html('#ajax_circuit_simulation_details', '');
		$commands[] = ajax_command_html('#ajax_selected_circuit_simulation', '');
		$commands[] = ajax_command_html('#ajax_selected_circuit_simulation_esim', '');
		$commands[] = ajax_command_data('#ajax_selected_circuit_simulation', 'form_state_value_select', $form_state['values']['circuit_simulation']);
	}
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}
function bootstrap_table_format($headers, $rows)
{
	$thead = "";
	$tbody = "";
	foreach ($headers as $header) {
		$thead .= "<th>{$header}</th>";
	} //$headers as $header
	foreach ($rows as $row) {
		$tbody .= "<tr>";
		foreach ($row as $data) {
			$tbody .= "<td>{$data}</td>";
		} //$row as $data
		$tbody .= "</tr>";
	} //$rows as $row
	$table = "
            <table class='table table-bordered table-hover' style='margin-left:-140px'>
                <thead>{$thead}</thead>
                <tbody>{$tbody}</tbody>
            </table>
        ";
	return $table;
}
/*****************************************************/
function _list_of_circuit_simulation()
{
	$circuit_simulation_titles = array(
		'0' => 'Please select...'
	);
	//$lab_titles_q = db_query("SELECT * FROM {circuit_simulation_proposal} WHERE solution_display = 1 ORDER BY lab_title ASC");
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('approval_status', 3);
	$query->orderBy('project_title', 'ASC');
	$circuit_simulation_titles_q = $query->execute();
	while ($circuit_simulation_titles_data = $circuit_simulation_titles_q->fetchObject()) {
		$circuit_simulation_titles[$circuit_simulation_titles_data->id] = $circuit_simulation_titles_data->project_title . ' (Proposed by ' . $circuit_simulation_titles_data->name_title . ' ' . $circuit_simulation_titles_data->contributor_name . ')';
	} //$circuit_simulation_titles_data = $circuit_simulation_titles_q->fetchObject()
	return $circuit_simulation_titles;
}
function _circuit_simulation_information($proposal_id)
{
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $proposal_id);
	$query->condition('approval_status', 3);
	$circuit_simulation_q = $query->execute();
	$circuit_simulation_data = $circuit_simulation_q->fetchObject();
	if ($circuit_simulation_data) {
		return $circuit_simulation_data;
	} //$circuit_simulation_data
	else {
		return 'Not found';
	}
}
function _circuit_simulation_details($circuit_simulation_default_value)
{
	$circuit_simulation_details = _circuit_simulation_information($circuit_simulation_default_value);
	if ($circuit_simulation_default_value != 0) {
		if($circuit_simulation_details->reference != NULL){
		$url = '~(?:(https?)://([^\s<]+)|(www\.[^\s<]+?\.[^\s<]+))(?<![\.,:])~i';
	        $reference = preg_replace($url, '<a href="$0" target="_blank" title="$0">$0</a>', $circuit_simulation_details->reference);
		}else{
		$reference = 'Not provided';
		}
		$form['circuit_simulation_details']['#markup'] = '<span style="color: rgb(128, 0, 0);"><strong>About the Circuit Simulation</strong></span></td><td style="width: 35%;"><br />' . '<ul>' . '<li><strong>Proposer Name:</strong> ' . $circuit_simulation_details->name_title . ' ' . $circuit_simulation_details->contributor_name . '</li>' . '<li><strong>Title of the Circuit Simulation:</strong> ' . l($circuit_simulation_details->project_title,'circuit-simulation-project/full-download/project/' . $circuit_simulation_default_value) . '</li>' . '<li><strong>University:</strong> ' . $circuit_simulation_details->university . '</li>' . '<li>'.'<strong>Reference:</strong> ' . $reference .'</li>'.'</ul>';
		$details = $form['circuit_simulation_details']['#markup'];
		return $details;
	} //$circuit_simulation_default_value != 0
}
