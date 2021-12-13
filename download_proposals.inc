<?php

function esim_circuit_simulation_download_completed_proposals()
{
	$output = "";
	$output .= "Click ".l("here","/circuit-simulation-project/download-proposals"). " to download the Proposals of the participants" ."<h4>";

	return $output;

}